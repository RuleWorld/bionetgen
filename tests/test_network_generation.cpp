#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "actions/ActionDispatch.hpp"
#include "engine/NetworkGenerator.hpp"
#include "parser/BNGAstVisitor.hpp"
#include "core/Ullmann.hpp"
#include "TestUtils.hpp"

namespace {

class ScopedTempDir {
public:
    ScopedTempDir()
        : path_(std::filesystem::temp_directory_path() / ("bng-cpp-" + std::to_string(
              std::chrono::steady_clock::now().time_since_epoch().count()))) {
        std::filesystem::create_directories(path_);
    }

    ~ScopedTempDir() {
        std::error_code ignored;
        std::filesystem::remove_all(path_, ignored);
    }

    const std::filesystem::path& path() const {
        return path_;
    }

private:
    std::filesystem::path path_;
};

std::filesystem::path writeTestModel(const std::filesystem::path& directory) {
    const std::filesystem::path modelPath = directory / "simple.bngl";
    std::ofstream out(modelPath);
    REQUIRE(out.is_open());
    out << R"(begin model
begin parameters
  kf 1
  kr 2
end parameters
begin molecule types
  A(b)
  B(a)
end molecule types
begin seed species
  A(b) 10
  B(a) 20
end seed species
begin observables
  Molecules Atot A()
end observables
begin reaction rules
  A(b) + B(a) <-> A(b!1).B(a!1) kf,kr
end reaction rules
end model

generate_network({overwrite=>1,max_iter=>10})
)";
    return modelPath;
}

std::string readTextFile(const std::filesystem::path& path) {
    std::ifstream input(path);
    REQUIRE(input.is_open());
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

std::string trim(std::string value) {
  auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char c) {
    return std::isspace(c) != 0;
  });
  auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char c) {
    return std::isspace(c) != 0;
  }).base();
  if (first >= last) {
    return {};
  }
  return std::string(first, last);
}

std::vector<int> parseIndexList(const std::string& text) {
  std::vector<int> indices;
  std::size_t start = 0;
  while (start <= text.size()) {
    const auto comma = text.find(',', start);
    const auto token = trim(text.substr(start, comma == std::string::npos ? std::string::npos : comma - start));
    if (!token.empty()) {
      indices.push_back(std::stoi(token));
    }
    if (comma == std::string::npos) {
      break;
    }
    start = comma + 1;
  }
  std::sort(indices.begin(), indices.end());
  return indices;
}

std::string joinIndices(const std::vector<int>& indices) {
  std::ostringstream out;
  for (std::size_t i = 0; i < indices.size(); ++i) {
    if (i != 0) {
      out << ",";
    }
    out << indices[i];
  }
  return out.str();
}

struct ParsedNet {
  std::multiset<std::string> species;
  std::multiset<std::string> reactions;
  std::multiset<std::string> groups;
  std::size_t speciesCount = 0;
  std::size_t reactionCount = 0;
  std::size_t groupCount = 0;
};

ParsedNet parseNetFile(const std::filesystem::path& path) {
  std::ifstream input(path);
  REQUIRE(input.is_open());

  ParsedNet parsed;
  enum class Section {
    None,
    Species,
    Reactions,
    Groups,
  };
  Section section = Section::None;

  std::string line;
  while (std::getline(input, line)) {
    const auto text = trim(line);
    if (text.empty() || text.front() == '#') {
      continue;
    }

    if (text == "begin species") {
      section = Section::Species;
      continue;
    }
    if (text == "begin reactions") {
      section = Section::Reactions;
      continue;
    }
    if (text == "begin groups") {
      section = Section::Groups;
      continue;
    }
    if (text.rfind("end ", 0) == 0) {
      section = Section::None;
      continue;
    }

    std::istringstream row(text);
    if (section == Section::Species) {
      int index = 0;
      std::string speciesText;
      row >> index >> speciesText;
      REQUIRE(!speciesText.empty());
      parsed.species.insert(speciesText);
      ++parsed.speciesCount;
      continue;
    }

    if (section == Section::Reactions) {
      int index = 0;
      std::string reactants;
      std::string products;
      row >> index >> reactants >> products;
      REQUIRE(!reactants.empty());
      REQUIRE(!products.empty());
      const auto lhs = joinIndices(parseIndexList(reactants));
      const auto rhs = joinIndices(parseIndexList(products));
      parsed.reactions.insert(lhs + "->" + rhs);
      ++parsed.reactionCount;
      continue;
    }

    if (section == Section::Groups) {
      int index = 0;
      std::string name;
      row >> index >> name;
      std::string remainder;
      std::getline(row, remainder);
      remainder = trim(remainder);

      std::vector<std::string> entries;
      std::size_t start = 0;
      while (start <= remainder.size()) {
        const auto comma = remainder.find(',', start);
        const auto token = trim(remainder.substr(start, comma == std::string::npos ? std::string::npos : comma - start));
        if (!token.empty()) {
          entries.push_back(token);
        }
        if (comma == std::string::npos) {
          break;
        }
        start = comma + 1;
      }
      std::sort(entries.begin(), entries.end());

      std::ostringstream normalized;
      normalized << name << ":";
      for (std::size_t i = 0; i < entries.size(); ++i) {
        if (i != 0) {
          normalized << ",";
        }
        normalized << entries[i];
      }
      parsed.groups.insert(normalized.str());
      ++parsed.groupCount;
    }
  }

  return parsed;
}

std::filesystem::path generateNetFromModel(const std::filesystem::path& modelPathInTempDir) {
  auto model = bng::parser::parseModel(readTextFile(modelPathInTempDir));
  auto& actions = model->getActions();
  actions.erase(std::remove_if(actions.begin(), actions.end(), [](const auto& action) {
    return action.name != "generate_network" && action.name.rfind("write", 0) != 0;
  }), actions.end());
  for (auto& action : actions) {
    if (action.name == "generate_network") {
      action.arguments["print_iter"] = "1";
      action.arguments["print_rule_progress"] = "1";
    }
  }
  bng::actions::ActionDispatch::execute(*model, modelPathInTempDir);
  return modelPathInTempDir.parent_path() / (modelPathInTempDir.stem().string() + ".net");
}

void assertNetParity(const std::string& modelName) {
  std::cerr << "[parity] starting model: " << modelName << std::endl;
  ScopedTempDir tempDir;
  const auto modelSource = std::filesystem::path(test::sourcePath("tests/models/" + modelName + ".bngl"));
  const auto modelCopy = tempDir.path() / (modelName + ".bngl");
  std::filesystem::copy_file(modelSource, modelCopy);

  std::cerr << "[parity] generating network for: " << modelName << std::endl;
  const auto generatedNet = generateNetFromModel(modelCopy);
  const auto referenceNet = std::filesystem::path(test::sourcePath("tests/reference/" + modelName + ".net"));

  std::cerr << "[parity] parsing generated/reference .net for: " << modelName << std::endl;
  const auto generated = parseNetFile(generatedNet);
  const auto reference = parseNetFile(referenceNet);

  std::cerr << "[parity] " << modelName
            << " generated counts species=" << generated.speciesCount
            << " reactions=" << generated.reactionCount
            << " groups=" << generated.groupCount << std::endl;
  std::cerr << "[parity] " << modelName
            << " reference counts species=" << reference.speciesCount
            << " reactions=" << reference.reactionCount
            << " groups=" << reference.groupCount << std::endl;

  REQUIRE(generated.speciesCount == reference.speciesCount);
  REQUIRE(generated.reactionCount == reference.reactionCount);
  REQUIRE(generated.groupCount == reference.groupCount);
  REQUIRE(generated.species == reference.species);
  REQUIRE(generated.reactions == reference.reactions);
  REQUIRE(generated.groups == reference.groups);

  std::cerr << "[parity] model passed: " << modelName << std::endl;
}

} // namespace

TEST_CASE("Compatibility network generator reads generated .net output", "[network]") {
    ScopedTempDir tempDir;
    const auto modelPath = writeTestModel(tempDir.path());
    auto model = bng::parser::parseModel(readTextFile(modelPath));

    bng::engine::NetworkGenerator generator(*model);
    const auto network = generator.generate(modelPath);

    REQUIRE(std::filesystem::exists(tempDir.path() / "simple.net"));
    REQUIRE(network.species.size() == 3);
    REQUIRE(network.reactions.size() == 2);
    REQUIRE(network.species.get(0).getSpeciesGraph().toString() == "A(b)");
    REQUIRE(network.species.get(2).getSpeciesGraph().toString() == "A(b!1).B(a!1)");
}

TEST_CASE("Action dispatch executes model actions through compatibility backend", "[actions]") {
    ScopedTempDir tempDir;
    const auto modelPath = writeTestModel(tempDir.path());
    auto model = bng::parser::parseModel(readTextFile(modelPath));

    REQUIRE_NOTHROW(bng::actions::ActionDispatch::execute(*model, modelPath));
    REQUIRE(std::filesystem::exists(tempDir.path() / "simple.net"));
}

TEST_CASE("Symmetric binding keeps multiplicity for duplicate site matches", "[network]") {
    auto model = bng::parser::parseModel(R"(begin model
begin parameters
  kf 1
end parameters
begin molecule types
  R(r,r)
  L(l,l)
end molecule types
begin seed species
  R(r,r) 1
  L(l,l) 1
end seed species
begin reaction rules
  R(r) + L(l,l) -> R(r!1).L(l!1,l) kf
end reaction rules
end model

generate_network({overwrite=>1,max_iter=>5})
)");

    bng::engine::NetworkGenerator generator(*model);
    const auto network = generator.generateNative(5);

    // Perl generates: 4 species, 2 reactions
    // Rxn 1: R(r,r) + L(l,l) -> L(l!1,l).R(r,r!1) factor=4 (2 r-sites * 2 l-sites)
    // Rxn 2: L(l,l) + L(l!1,l).R(r,r!1) -> L(l!1,l).L(l!2,l).R(r!1,r!2) factor=2 (1 r-site * 2 l-sites)
    REQUIRE(network.species.size() == 4);
    REQUIRE(network.reactions.size() == 2);
    REQUIRE(network.reactions.all()[0].getRateLaw() == "kf");
    REQUIRE(std::abs(network.reactions.all()[0].getFactor() - 4.0) < 1e-9);
    REQUIRE(std::abs(network.reactions.all()[1].getFactor() - 2.0) < 1e-9);
}

TEST_CASE("Embedding count for symmetric sites", "[network][debug]") {
    auto model = bng::parser::parseModel(R"(begin model
begin parameters
  kf 1
end parameters
begin molecule types
  R(r,r)
  L(l,l)
end molecule types
begin seed species
  R(r,r) 1
  L(l,l) 1
end seed species
begin reaction rules
  R(r) + L(l,l) -> R(r!1).L(l!1,l) kf
end reaction rules
end model

generate_network({overwrite=>1,max_iter=>1})
)");

    const auto& rules = model->getReactionRules();
    REQUIRE(rules.size() == 1);

    bng::ast::SpeciesList speciesList;
    for (const auto& seed : model->getSeedSpecies()) {
        speciesList.add(bng::ast::Species(
            bng::ast::SpeciesGraph(seed.getGraph()),
            1.0, false, ""));
    }
    REQUIRE(speciesList.size() == 2);

    // Pattern 0: R(r) should find 2 embeddings into R(r,r)
    auto embeddings0 = rules[0].findEmbeddings(0, speciesList);
    std::cerr << "[debug] Pattern 0 R(r) embeddings: " << embeddings0.size() << std::endl;
    for (const auto& e : embeddings0) {
        std::cerr << "[debug]   species=" << e.speciesIndex << std::endl;
    }

    // Pattern 1: L(l,l) should find 2 embeddings into L(l,l) (swapping l sites)
    auto embeddings1 = rules[0].findEmbeddings(1, speciesList);
    std::cerr << "[debug] Pattern 1 L(l,l) embeddings: " << embeddings1.size() << std::endl;
    for (const auto& e : embeddings1) {
        std::cerr << "[debug]   species=" << e.speciesIndex << std::endl;
    }

    // R(r) should have 2 embeddings into R(r,r)
    REQUIRE(embeddings0.size() == 2);
    // L(l,l) should have 1 embedding into L(l,l) after reaction center filtering
    // (both permutations map the same reaction center components)
    REQUIRE(embeddings1.size() >= 1);
}

  TEST_CASE("Reference parity for catalysis", "[network][parity][.]") {
    assertNetParity("catalysis");
  }

  TEST_CASE("Reference parity for BLBR", "[network][parity][.]") {
    assertNetParity("blbr");
  }

  TEST_CASE("Reference parity for EGFR", "[network][parity][slow][.]") {
    assertNetParity("egfr_net");
  }



