#pragma once
// MacroBNGModel.hpp — faithful port of Perl BNG2 MacroBNGModel.pm
// Performs model reduction by:
//   1. Analyzing binding site dependencies across rules
//   2. Identifying minimal sets of macro variables
//   3. Transforming micro-rules into macro-rules
//   4. Transforming species and observables
//   5. Correcting the network file

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace bng {
namespace ast {

class Model; // forward

/// MacroBNGModel — 1:1 port of MacroBNGModel.pm (2087 lines).
///
/// Perl global / closure-scoped variables are stored as members.
/// Every Perl subroutine becomes a method with the same name.
class MacroBNGModel {
public:
    MacroBNGModel();
    ~MacroBNGModel();

    // ---- main entry (Perl readFile, lines 31-98) ----
    std::string readFile(const std::map<std::string, std::string>& params);

    // ---- preprocessor (Perl pre_macr, lines 104-371) ----
    std::string pre_macr(const std::string& param_prefix);

    // ---- species pre-processor (Perl pre_species1, lines 375-407) ----
    void pre_species1(std::map<std::string, int>& nm_site,
                      std::map<std::string, int>& nm2_site);

    // ---- rules pre-processor (Perl pre_rules, lines 410-437) ----
    void pre_rules(std::map<std::string, int>& nm_site,
                   std::map<std::string, int>& nm2_site,
                   const std::string& param_prefix,
                   std::map<std::string, std::vector<std::string>>& skf,
                   std::map<std::string, std::vector<std::string>>& dpp_site,
                   std::map<std::string, std::vector<int>>& lis1h);

    // ---- blank-line remover (Perl del_blank, lines 439-460) ----
    void del_blank(const std::vector<std::string>& str,
                   std::vector<std::string>& lis1);

    // ---- hash_skf (lines 462-493) ----
    void hash_skf(const std::vector<std::string>& lis1,
                  std::map<std::string, std::vector<std::string>>& skf,
                  const std::map<std::string, int>& nm2_site,
                  const std::string& label = "");

    // ---- skf0 (lines 877-918) ----
    void skf0(const std::string& rp1,
              std::map<std::string, std::vector<std::string>>& skf,
              const std::map<std::string, int>& nm2_site);

    // ---- add_skf (lines 854-875) ----
    void add_skf(const std::string& skf1, const std::string& skf2,
                 const std::string& sit1,
                 std::map<std::string, std::vector<std::string>>& skf,
                 const std::map<std::string, int>& nm2_site);

    // ---- inc_elt (lines 496-509) ----
    int inc_elt(const std::string& re, const std::vector<std::string>& pr);

    // ---- inc_set (lines 511-526) ----
    bool inc_set(const std::vector<std::string>& re,
                 const std::vector<std::string>& pr);

    // ---- del_set (lines 528-543) ----
    void del_set(const std::vector<std::string>& rem,
                 const std::vector<std::string>& prm,
                 std::vector<std::string>& rem_prm);

    // ---- activ_sit (lines 563-580) ----
    void activ_sit(int typrul, std::string& reac, std::string& prod,
                   std::vector<std::string>& mreac);

    // ---- hierarch_sit (lines 582-627) ----
    void hierarch_sit(std::map<std::string, std::vector<std::string>>& dpp_site);

    // ---- min_set (lines 630-698) ----
    void min_set(std::map<std::string, std::vector<std::string>>& dpp_site,
                 std::map<std::string, std::vector<int>>& lis1h,
                 const std::map<std::string, int>& nm_site,
                 const std::map<std::string, int>& nm2_site,
                 std::map<std::string, int>& site_lig);

    // ---- num_site (lines 701-760) ----
    std::string num_site(const std::string& re, const std::string& pr,
                         const std::map<std::string, int>& nm2_site,
                         std::vector<std::string>& dep_sit);

    // ---- hash_sor (lines 763-851) ----
    void hash_sor(const std::vector<std::string>& lis1,
                  const std::map<std::string, int>& nm2_site,
                  std::map<std::string, std::vector<int>>& lis1h,
                  std::map<std::string, std::vector<std::string>>& dpp_site);

    // ---- trans_rec (lines 1210-1239) ----
    void trans_rec(std::map<std::string, std::vector<std::string>>& dpp_site,
                   std::map<std::string, std::vector<int>>& lis1h,
                   const std::vector<std::string>& lis1,
                   std::map<std::string, int>& site_lig);

    // ---- r_mac (lines 1304-1387) ----
    void r_mac(std::map<std::string, std::vector<std::string>>& dpp_site,
               std::map<std::string, std::vector<int>>& lis1h,
               const std::string& value,
               std::map<std::string, int>& site_lig,
               int ind,
               std::vector<std::string>& li);

    // ---- pre_obs1 (lines 1390-1441) ----
    void pre_obs1(std::map<std::string, int>& nm_site,
                  std::map<std::string, int>& nm2_site,
                  const std::string& param_prefix,
                  std::map<std::string, std::vector<std::string>>& skf,
                  std::map<std::string, std::vector<std::string>>& dpp_site,
                  std::map<std::string, std::vector<int>>& lis1h);

    // ---- trans_obs (lines 1242-1301) ----
    void trans_obs(std::map<std::string, std::vector<std::string>>& dpp_site,
                   std::map<std::string, std::vector<int>>& lis1h,
                   const std::vector<std::string>& lis1,
                   std::map<std::string, std::vector<std::string>>& skf);

    // ---- obs_mac0 (lines 1443-1520) ----
    std::string obs_mac0(const std::string& comp,
                         std::map<std::string, std::vector<std::string>>& dpp_site,
                         std::map<std::string, std::vector<int>>& lis1h,
                         std::map<std::string, std::vector<std::string>>& skf);

    // ---- obs_mac (lines 1523-1622) ----
    void obs_mac(const std::string& comp,
                 std::map<std::string, std::vector<std::string>>& dpp_site,
                 std::map<std::string, std::vector<int>>& lis1h,
                 std::string& mol,
                 std::string& mol_form,
                 std::map<std::string, std::vector<std::string>>& skf);

    // ---- trans_specie (lines 1625-1705) ----
    void trans_specie(std::map<std::string, std::vector<std::string>>& dpp_site,
                      std::map<std::string, std::vector<int>>& lis1h,
                      const std::vector<std::string>& lis1);

    // ---- cor_net (lines 921-1207) ----
    void cor_net(const std::string& param_prefix);

    // ---- L_AiAn_BiBn (lines 1782-1822) ----
    void L_AiAn_BiBn(std::map<std::string, std::vector<std::string>>& croc,
                      std::map<std::string, std::string>& ms,
                      std::vector<std::string>& rabn,
                      std::vector<std::string>& rabi,
                      std::vector<std::string>& reac,
                      int& n_adds,
                      std::map<std::string, int>& dubl);

    // ---- R_AiAn_BiBn (lines 1987-2018) ----
    void R_AiAn_BiBn(std::map<std::string, std::vector<std::string>>& croc,
                      std::map<std::string, std::string>& ms,
                      std::vector<std::string>& rabn,
                      std::vector<std::string>& rabi,
                      std::vector<std::string>& reac,
                      int& n_subtracts,
                      std::map<std::string, int>& dubl);

    // ---- cros (lines 1825-1894) ----
    bool cros(std::map<std::string, std::vector<std::string>>& croc,
              std::map<std::string, std::string>& ms,
              int ai, int an);

    // ---- delsites (lines 1897-1975) ----
    void delsites(std::vector<std::string>& rr1,
                  const std::string& teil1_in,
                  const std::vector<std::string>& aonly,
                  std::vector<std::string>& rr1out,
                  std::string& tei1out);

    // ---- selectmet (lines 2020-2043) ----
    void selectmet(std::vector<std::string>& met,
                   const std::string& tei,
                   const std::string& me);

    // ---- add_ts3n (lines 1978-1985) ----
    void add_ts3n(int an, int bn);

    // ---- subTrash (referenced line 1133, missing in Perl) ----
    void subTrash(std::vector<std::string>& rabn,
                  std::vector<std::string>& reac);

    // ---- spere (referenced line 1563, missing in Perl) ----
    void spere(std::vector<std::string>& pere,
               std::vector<std::string>& obed,
               const std::vector<std::string>& rab2);

    // ---- h_dep (lines 2046-2078) ----
    void h_dep(std::map<std::string, std::vector<std::string>>& dpp_site,
               std::map<std::string, int>& dep_del,
               const std::vector<std::string>& rabm,
               std::vector<std::string>& rab2,
               const std::string& out);

    // ---- line I/O helpers (Perl get_line / read_block_array / errgen) ----
    std::string get_line();
    std::pair<std::vector<std::pair<std::string,int>>, std::string>
        read_block_array(const std::string& name);
    std::string errgen(const std::string& err, int lno = -1);

    // ---- version ----
    std::string version() const;

    /// Returns true if the macro model has been populated.
    bool empty() const { return species2_.empty() && reac1tion_.empty(); }

private:
    // ---- Perl package-level / closure-scoped state ----
    bool NO_EXEC_ = false;
    int line_number_ = 0;
    std::vector<std::string> file_dat_;
    std::string file_;             // current filename

    // data accumulated during pre_macr
    std::vector<std::string> species2_;
    std::vector<std::string> reac1tion_;
    std::vector<std::string> obser1vable_;
    std::vector<std::string> gene1rate_;
    std::map<std::string, std::string> para1ms_;

    // data accumulated during cor_net
    std::map<std::string, int> ts3n_;      // key = "an.bn"
    std::vector<std::string> species_;     // species list from .net
    std::map<std::string, std::vector<std::string>> egf_tot_;

    // simulation / generate strings
    std::string gener1_;
    std::string simul1_;

    // temporary vars used across L_AiAn_BiBn / R_AiAn_BiBn / cros
    int tnnn_ = 0;
    int tnni_ = 0;

    // log / record streams (written to *.rec, *.rules, *.obser, *.spec2)
    std::ofstream WFILErec_;
    std::ofstream WFILErules_;
    std::ofstream WFILEobser_;
    std::ofstream WFILEspec2_;

    // internal helpers
    static std::vector<std::string> split(const std::string& s, char delim);
    static std::vector<std::string> split(const std::string& s,
                                          const std::string& delim);
    static std::string join(const std::string& delim,
                            const std::vector<std::string>& v);
    static bool startsWith(const std::string& s, const std::string& prefix);
    static bool endsWith(const std::string& s, const std::string& suffix);
    static std::string replaceFirst(const std::string& s,
                                    const std::string& from,
                                    const std::string& to);
    static std::string replaceAll(const std::string& s,
                                  const std::string& from,
                                  const std::string& to);
    static std::string ltrim(const std::string& s);
    static std::string rtrim(const std::string& s);
    static std::string trim(const std::string& s);
    // Perl quotemeta-like escape for use with regex
    static std::string quotemeta(const std::string& s);
};

} // namespace ast
} // namespace bng
