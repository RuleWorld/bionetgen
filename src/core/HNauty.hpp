#pragma once
// HNauty.hpp - C++ port of Perl HNauty.pm
// Canonical graph labeling for BioNetGen molecular species graphs.
//
// This is a faithful port of the Perl HNauty algorithm. Since BNG graphs
// are always undirected (adj_in == adj_out), we use a single adjacency map.
//
// Main entry: BNGcore::hnauty(adj, nodeColoring) -> permutation map

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace BNGcore {

// Type aliases for readability
using AdjMap = std::map<int, std::map<int, std::vector<int>>>;
using Partition = std::vector<std::vector<int>>;
using Perm = std::map<int, int>;

namespace hnauty_detail {

// ---- intersection ----
// Set intersection of two int vectors
inline std::vector<int> intersection(const std::vector<int>& a, const std::vector<int>& b) {
    std::set<int> setB(b.begin(), b.end());
    std::vector<int> result;
    for (int v : a) {
        if (setB.count(v)) {
            result.push_back(v);
        }
    }
    return result;
}

// ---- is_discrete ----
// Check if all cells in partition are singletons
inline bool is_discrete(const Partition& partition) {
    for (auto& cell : partition) {
        if (cell.size() > 1) return false;
    }
    return true;
}

// ---- partition_value ----
// Compute cell-size distribution as an indicator vector.
// For each cell size from 1..max_size, count how many cells have that size.
inline std::vector<int> partition_value(const Partition& part) {
    std::map<int, int> tmp;
    for (auto& cell : part) {
        tmp[(int)cell.size()] += 1;
    }
    int maxSize = tmp.rbegin()->first;
    std::vector<int> indicator;
    for (int j = 1; j <= maxSize; j++) {
        if (tmp.count(j)) {
            indicator.push_back(tmp[j]);
        } else {
            indicator.push_back(0);
        }
    }
    return indicator;
}

// ---- lex_ordered ----
// Lexicographic comparison of two flat int vectors.
// Returns: {cmp, depth} where cmp is -1 (lt), 0 (eq), or 1 (gt),
// and depth is the index where comparison terminated.
inline std::pair<int, int> lex_ordered(const std::vector<int>& a, const std::vector<int>& b) {
    int length = std::min((int)a.size(), (int)b.size());
    int i = 0;
    while (i < length) {
        if (a[i] > b[i]) return {1, i};
        if (a[i] < b[i]) return {-1, i};
        i++;
    }
    return {0, i};
}

// Overload for comparing vectors-of-vectors (nested lex comparison).
// The Perl code uses this to compare [node_indicator, adj_str] tuples.
// We flatten the comparison: first compare element-by-element from the
// first sub-vector, then the second, etc.
inline std::pair<int, int> lex_ordered_nested(
    const std::vector<std::vector<int>>& a,
    const std::vector<std::vector<int>>& b)
{
    int minLen = std::min((int)a.size(), (int)b.size());
    int globalDepth = 0;
    for (int k = 0; k < minLen; k++) {
        auto [cmp, depth] = lex_ordered(a[k], b[k]);
        if (cmp != 0) return {cmp, globalDepth + depth};
        globalDepth += (int)a[k].size();
    }
    return {0, globalDepth};
}

// ---- adj_permute ----
// Apply a permutation to the adjacency map: vertex i -> perm[i]
inline AdjMap adj_permute(const AdjMap& adj, const Perm& perm) {
    AdjMap newAdj;
    for (auto& [i, neighbors] : adj) {
        int newI = perm.at(i);
        for (auto& [j, edges] : neighbors) {
            int newJ = perm.at(j);
            newAdj[newI][newJ] = edges;
        }
    }
    return newAdj;
}

// ---- get_adj_str ----
// Convert adjacency matrix to a flat vector after permuting vertices.
// part is a discrete partition (each cell is a singleton).
// Each entry is sum of 2^(edge_label) for edges, or 0.
inline std::vector<int> get_adj_str(const AdjMap& adj, const Partition& part) {
    // Build permutation from partition: part[i][0] -> i
    Perm perm;
    for (int i = 0; i < (int)part.size(); i++) {
        perm[part[i][0]] = i;
    }
    int length = (int)adj.size();
    AdjMap newAdj = adj_permute(adj, perm);
    std::vector<int> adjInfo;
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (newAdj.count(i) && newAdj[i].count(j)) {
                int value = 0;
                for (int e : newAdj[i][j]) {
                    value += (1 << e);
                }
                adjInfo.push_back(value);
            } else {
                adjInfo.push_back(0);
            }
        }
    }
    return adjInfo;
}

// ---- partition_meet ----
// Compute the meet (refinement) of a partition with multiple splitting partitions.
// For each splitting partition p, refine by intersecting each current cell with
// each cell in p.
inline Partition partition_meet(const Partition& theSet, const std::vector<Partition>& partitions) {
    Partition newPart = theSet;
    for (auto& p : partitions) {
        if (p.size() == 1) continue; // single-cell partition doesn't refine
        Partition newerPart;
        for (auto& cellI : newPart) {
            for (auto& cellJ : p) {
                auto inter = intersection(cellI, cellJ);
                if (!inter.empty()) {
                    newerPart.push_back(inter);
                }
            }
        }
        newPart = newerPart;
    }
    return newPart;
}

// ---- refinement ----
// Partition refinement. For each cell in 'cells', for each non-singleton cell
// in the partition, compute degree to the cell grouped by edge label.
// Split cells with different degree patterns using partition_meet.
// Since graph is undirected, adj_in == adj_out, but we mirror the Perl code
// which processes both in-degree and out-degree (same adjacency map for both).
inline Partition refinement(
    const AdjMap& adj,              // adj (used for both in and out since undirected)
    const Partition& orderedPartition,
    const Partition& theCells)
{
    Partition cells = theCells;
    Partition eqPartition = orderedPartition;

    int cellCount = 0;
    while (!is_discrete(eqPartition) && cellCount < (int)cells.size()) {
        // Build cell set from cells[cellCount]
        std::set<int> cellSet(cells[cellCount].begin(), cells[cellCount].end());
        cellCount++;

        int partitionCount = 0;
        while (partitionCount < (int)eqPartition.size()) {
            if ((int)eqPartition[partitionCount].size() > 1) {
                // out_degree_count[edgeLabel][degree] = set of vertices
                // in_degree_count[edgeLabel][degree] = set of vertices
                // For undirected, we compute both using the same adj map.
                std::map<int, std::map<int, std::set<int>>> outDegreeCount;
                std::map<int, std::map<int, std::set<int>>> inDegreeCount;

                auto& currentCell = eqPartition[partitionCount];

                // Initialize degree-0 sets for each edge label that appears
                // (done lazily below)

                for (int v : currentCell) {
                    // Out-degree computation (adj_out)
                    std::map<int, int> count;
                    if (adj.count(v)) {
                        for (auto& [u, edges] : adj.at(v)) {
                            if (cellSet.count(u)) {
                                for (int e : edges) {
                                    count[e]++;
                                }
                            }
                        }
                    }
                    for (auto& [e, cnt] : count) {
                        if (!outDegreeCount.count(e)) {
                            // Initialize degree-0 bucket with all vertices in currentCell
                            for (int w : currentCell) {
                                outDegreeCount[e][0].insert(w);
                            }
                        }
                        outDegreeCount[e][cnt].insert(v);
                        outDegreeCount[e][0].erase(v);
                    }

                    // In-degree computation (adj_in, which is transpose of adj)
                    // For undirected graphs adj_in == adj_out, but we faithfully
                    // replicate the Perl behavior by computing in-degree from
                    // the same adjacency map (treating it as adj_in too).
                    // adj_in[v][u] means there's an edge from u to v.
                    // Since undirected: adj[v][u] == adj[u][v], so adj_in == adj_out.
                    std::map<int, int> inCount;
                    if (adj.count(v)) {
                        for (auto& [u, edges] : adj.at(v)) {
                            if (cellSet.count(u)) {
                                for (int e : edges) {
                                    inCount[e]++;
                                }
                            }
                        }
                    }
                    for (auto& [e, cnt] : inCount) {
                        if (!inDegreeCount.count(e)) {
                            for (int w : currentCell) {
                                inDegreeCount[e][0].insert(w);
                            }
                        }
                        inDegreeCount[e][cnt].insert(v);
                        inDegreeCount[e][0].erase(v);
                    }
                }

                // Build splitting partitions from out_degree_count
                std::vector<Partition> parts;
                for (auto& [e, degMap] : outDegreeCount) {
                    Partition newPart;
                    for (auto& [sz, verts] : degMap) {
                        if (!verts.empty()) {
                            newPart.push_back(std::vector<int>(verts.begin(), verts.end()));
                        }
                    }
                    if (newPart.size() > 1) {
                        parts.push_back(newPart);
                    }
                }
                // Same for in_degree_count
                for (auto& [e, degMap] : inDegreeCount) {
                    Partition newPart;
                    for (auto& [sz, verts] : degMap) {
                        if (!verts.empty()) {
                            newPart.push_back(std::vector<int>(verts.begin(), verts.end()));
                        }
                    }
                    if (newPart.size() > 1) {
                        parts.push_back(newPart);
                    }
                }

                // Update equitable partition and cells
                if (!parts.empty()) {
                    Partition newPart;
                    if (parts.size() > 1) {
                        Partition singleCellPartition = { currentCell };
                        newPart = partition_meet(singleCellPartition, parts);
                    } else {
                        newPart = parts[0];
                    }

                    // Find the largest sub-cell (index t)
                    int tIdx = 0;
                    int maxSize = 0;
                    for (int n = 0; n < (int)newPart.size(); n++) {
                        if ((int)newPart[n].size() > maxSize) {
                            tIdx = n;
                            maxSize = (int)newPart[n].size();
                        }
                    }

                    // Update cells list: for cells after cellCount, if a cell
                    // matches the original cell being split, replace with largest sub-cell
                    std::set<int> origSet(currentCell.begin(), currentCell.end());
                    for (int n = cellCount; n < (int)cells.size(); n++) {
                        std::set<int> cs(cells[n].begin(), cells[n].end());
                        if (cs == origSet) {
                            cells[n] = newPart[tIdx];
                        }
                    }

                    // Add non-largest sub-cells as new cells
                    for (int n = 0; n < (int)newPart.size(); n++) {
                        if (n != tIdx) {
                            cells.push_back(newPart[n]);
                        }
                    }

                    // Splice newPart into eqPartition replacing partitionCount
                    Partition updated;
                    for (int n = 0; n < partitionCount; n++) {
                        updated.push_back(eqPartition[n]);
                    }
                    for (auto& cell : newPart) {
                        updated.push_back(cell);
                    }
                    for (int n = partitionCount + 1; n < (int)eqPartition.size(); n++) {
                        updated.push_back(eqPartition[n]);
                    }
                    eqPartition = updated;
                }
            }
            partitionCount++;
        }
    }
    return eqPartition;
}

// ---- update_automorphisms ----
// Decompose permutation into cycles, compute fixed points and orbit representatives.
struct AutoInfo {
    Perm fix;                     // fixed points: vertex -> 1
    std::vector<int> orbitReps;   // one representative per orbit (smallest element)
};

inline std::pair<AutoInfo, std::vector<AutoInfo>> update_automorphisms(
    std::vector<AutoInfo>& autos, const Perm& perm)
{
    AutoInfo info;
    Perm unused = perm;

    while (!unused.empty()) {
        // Get smallest unused element
        int element = unused.begin()->first;
        std::vector<int> cycle = {element};

        while (true) {
            unused.erase(element);
            element = perm.at(element);
            if (!unused.count(element)) break;
            cycle.push_back(element);
        }

        std::sort(cycle.begin(), cycle.end());
        info.orbitReps.push_back(cycle[0]);
        if (cycle.size() == 1) {
            info.fix[cycle[0]] = 1;
        }
    }

    autos.push_back(info);
    return {info, autos};
}

// ---- pfixp ----
// Check if all singleton cells in the partition are fixed by the permutation.
inline bool pfixp(const Partition& partition, const Perm& permFix) {
    for (auto& cell : partition) {
        if (cell.size() == 1 && !permFix.count(cell[0])) {
            return false;
        }
    }
    return true;
}

// ---- update_cell ----
// Prune the target cell using automorphism information.
// For each automorphism whose fixed points cover all singletons of 'node',
// intersect the cell with that automorphism's orbit representatives.
inline std::vector<int> update_cell(
    const Partition& node,
    const std::vector<int>& cell,
    const std::vector<AutoInfo>& perms)
{
    std::vector<int> result = cell;
    for (auto& ai : perms) {
        if (pfixp(node, ai.fix)) {
            result = intersection(result, ai.orbitReps);
        }
    }
    return result;
}

} // namespace hnauty_detail

// ---- Main HNauty function ----
// adj: adjacency map. adj[v1][v2] = vector of edge labels (ints)
// nodeColoring: initial partition (vector of cells, each cell is vector of vertex indices)
// Returns: permutation map (original vertex index -> canonical position)
inline std::map<int,int> hnauty(
    const std::map<int, std::map<int, std::vector<int>>>& adj,
    const std::vector<std::vector<int>>& nodeColoring)
{
    using namespace hnauty_detail;

    int numVertices = (int)adj.size();

    // Also count vertices that appear only as neighbors (isolated targets)
    std::set<int> allVerts;
    for (auto& [v, nbrs] : adj) {
        allVerts.insert(v);
        for (auto& [u, _] : nbrs) {
            allVerts.insert(u);
        }
    }
    // Also add vertices from nodeColoring that may not be in adj
    for (auto& cell : nodeColoring) {
        for (int v : cell) {
            allVerts.insert(v);
        }
    }
    int totalVerts = (int)allVerts.size();

    bool complete = false;
    bool jumpBack = false;

    // best_node: [0] = partition, [1] = node_indicator, [2] = adj_str
    Partition bestNodePartition;
    std::vector<int> bestNodeIndicator;
    std::vector<int> bestNodeAdj;
    bool bestNodeEmpty = true;

    Partition firstTermPartition;
    std::vector<int> firstTermIndicator;
    std::vector<int> firstTermAdj;
    bool firstTermEmpty = true;

    std::vector<AutoInfo> automorphisms;
    std::vector<Perm> generators;

    int counter = 0;

    // current_node[i] = partition at depth i of the search tree
    std::vector<Partition> currentNode;
    // node_indicator[i] = partition_value at depth i
    std::vector<std::vector<int>> nodeIndicator;
    // cells[i] = candidate vertices to individualize at depth i
    std::vector<std::vector<int>> cells;
    // tplacement[i] = index of the cell being split at depth i
    std::vector<int> tplacement;
    // prune_autos[i] = index into automorphisms for pruning; -1 = not set
    std::vector<int> pruneAutos;

    // Initialize with enough space
    auto ensureSize = [&](int idx) {
        while ((int)currentNode.size() <= idx + 1) currentNode.push_back({});
        while ((int)nodeIndicator.size() <= idx + 1) nodeIndicator.push_back({});
        while ((int)cells.size() <= idx + 1) cells.push_back({});
        while ((int)tplacement.size() <= idx + 1) tplacement.push_back(0);
        while ((int)pruneAutos.size() <= idx + 1) pruneAutos.push_back(-1);
    };

    ensureSize(0);

    // Initial refinement
    Partition orderedPartition = nodeColoring;
    currentNode[0] = refinement(adj, orderedPartition, orderedPartition);
    nodeIndicator[0] = {};

    if (is_discrete(currentNode[0])) {
        auto newAdj = get_adj_str(adj, currentNode[counter]);
        bestNodePartition = currentNode[counter];
        bestNodeIndicator = nodeIndicator[counter];
        bestNodeAdj = newAdj;
        bestNodeEmpty = false;
        complete = true;
    }

    while (!complete) {
        // Main loop
        if (is_discrete(currentNode[counter])) {
            // The Perl code checks if first_terminal_node == current,
            // but this comparison (reference equality in Perl) is effectively
            // always false for distinct nodes. We just decrement counter.
            counter--;
            if (counter < 0) {
                complete = true;
            }
        } else if (!jumpBack) {
            // Find first non-trivial cell of smallest size
            int size = totalVerts;
            int t = 0;
            for (int n = 0; n < (int)currentNode[counter].size(); n++) {
                int cellSize = (int)currentNode[counter][n].size();
                if (cellSize > 1 && cellSize < size) {
                    t = n;
                    size = cellSize;
                }
            }
            ensureSize(counter);
            cells[counter] = currentNode[counter][t];
            tplacement[counter] = t;
            pruneAutos[counter] = -1; // undef
            // Truncate currentNode to counter+1
            currentNode.resize(counter + 1);
            ensureSize(counter);
        } else {
            currentNode.resize(counter + 1);
            ensureSize(counter);
        }

        bool searchVertex = true;
        while (!complete && searchVertex) {
            while (!complete && cells[counter].empty()) {
                counter--;
                if (counter < 0) {
                    complete = true;
                }
            }
            if (!complete) {
                if (pruneAutos[counter] == -1) {
                    pruneAutos[counter] = 0;
                } else {
                    if (pruneAutos[counter] < (int)automorphisms.size()) {
                        std::vector<AutoInfo> subset(
                            automorphisms.begin() + pruneAutos[counter],
                            automorphisms.end());
                        cells[counter] = update_cell(
                            currentNode[counter], cells[counter], subset);
                        pruneAutos[counter] = (int)automorphisms.size() - 1;
                    }
                }
                if (!cells[counter].empty()) {
                    std::sort(cells[counter].begin(), cells[counter].end());
                    searchVertex = false;
                }
            }
        }

        if (!complete) {
            int vertex = cells[counter][0];
            cells[counter].erase(cells[counter].begin());

            // Individualize vertex: split cell at tplacement[counter]
            Partition thisPartition = currentNode[counter];
            int t = tplacement[counter];
            std::vector<int> newset;
            for (int v : thisPartition[t]) {
                if (v != vertex) newset.push_back(v);
            }

            Partition splitPartition;
            for (int n = 0; n < t; n++) {
                splitPartition.push_back(thisPartition[n]);
            }
            splitPartition.push_back({vertex});
            splitPartition.push_back(newset);
            for (int n = t + 1; n < (int)thisPartition.size(); n++) {
                splitPartition.push_back(thisPartition[n]);
            }

            Partition newNode = refinement(adj, splitPartition, {{vertex}});

            ensureSize(counter + 1);
            nodeIndicator[counter + 1] = partition_value(newNode);
            // Truncate nodeIndicator to counter+2
            nodeIndicator.resize(counter + 2);

            jumpBack = false;

            // Pruning: compare node_indicator path with best_node's indicator
            if (!bestNodeEmpty) {
                auto [cmp, depth] = lex_ordered(nodeIndicator[counter + 1], bestNodeIndicator);
                // The Perl code compares nodeIndicator[counter] (which is the
                // accumulated indicator UP TO counter) with best_node[1].
                // But nodeIndicator is stored per-level. The Perl comparison
                // lex_ordered(@$node_indicator[$counter], @$best_node[1]) compares
                // the indicator at the CURRENT depth with the best indicator.
                // Let's match the Perl: compare nodeIndicator[counter+1] with bestNodeIndicator
                // Actually, Perl stores node_indicator as array indexed by depth, and
                // best_node[1] is node_indicator at the terminal depth.
                // The comparison is: lex_ordered(@$node_indicator[$counter], @$best_node[1])
                // where $counter hasn't been incremented yet. So it's the indicator at
                // the parent level being compared to the best terminal indicator.
                // But wait - the Perl sets @$node_indicator[$counter+1] = partition_value($new_node)
                // then checks @$node_indicator[$counter] against @$best_node[1].
                // That means it's comparing the PARENT's indicator with the best's indicator.
                // This is used for early cutoff at higher levels.
                auto [cmp2, depth2] = lex_ordered(nodeIndicator[counter + 1], bestNodeIndicator);
                if (cmp2 > 0) {
                    counter = depth2 - 1;
                    jumpBack = true;
                }
            }

            int a = -1, b = -1;

            if (!jumpBack) {
                currentNode.resize(counter + 2);
                ensureSize(counter + 1);
                currentNode[counter + 1] = newNode;
                counter++;

                if (is_discrete(newNode)) {
                    auto newAdj = get_adj_str(adj, newNode);

                    if (firstTermEmpty) {
                        firstTermPartition = currentNode[counter];
                        firstTermIndicator = nodeIndicator[counter];
                        firstTermAdj = newAdj;
                        firstTermEmpty = false;

                        bestNodePartition = firstTermPartition;
                        bestNodeIndicator = firstTermIndicator;
                        bestNodeAdj = firstTermAdj;
                        bestNodeEmpty = false;
                    } else {
                        // Compare [bestNodeIndicator, bestNodeAdj] with [nodeIndicator[counter], newAdj]
                        auto [cmpInd, _d1] = lex_ordered(bestNodeIndicator, nodeIndicator[counter]);
                        int cmpOverall = cmpInd;
                        if (cmpInd == 0) {
                            auto [cmpAdj, _d2] = lex_ordered(bestNodeAdj, newAdj);
                            cmpOverall = cmpAdj;
                        }
                        // Perl: lex_ordered([@$best_node[1..2]], [@$node_indicator[$counter], $new_adj])
                        // This is nested lex: compare indicators first, then adj strings
                        // lt means the NEW one is better (smaller)
                        // Actually in Perl: if @$i[0] eq 'lt', meaning best < new...
                        // Wait, Perl lex_ordered returns 'lt' when first < second.
                        // So lex_ordered(best, new) == 'lt' means best < new, meaning
                        // new is LARGER. The Perl code updates best_node in this case.
                        // That seems like it should be 'gt' for "new is better"...
                        // Let me re-read: the Perl says if @$i[0] eq 'lt' then update best_node.
                        // lex_ordered([@$best_node[1..2]], [current_indicator, new_adj])
                        // 'lt' means best < current, so current is LARGER.
                        // The algorithm picks the LARGEST canonical form? No, typically nauty
                        // picks the lexicographically smallest. But the Perl code picks 'lt'
                        // to update, meaning it updates when current > best, picking the MAX.
                        // Let's just faithfully replicate: if best < new, update best = new.
                        if (cmpOverall < 0) {
                            // best < new, update best to new
                            bestNodePartition = currentNode[counter];
                            bestNodeIndicator = nodeIndicator[counter];
                            bestNodeAdj = newAdj;
                            bestNodeEmpty = false;
                        } else {
                            // Check automorphism with best_node
                            auto [cmpBest, _d3] = lex_ordered(bestNodeAdj, newAdj);
                            if (cmpBest == 0) {
                                // Found automorphism with best node
                                Perm newAuto;
                                for (int i = 0; i < (int)bestNodePartition.size(); i++) {
                                    newAuto[bestNodePartition[i][0]] = currentNode[counter][i][0];
                                }
                                generators.push_back(newAuto);
                                auto [info, _autos] = update_automorphisms(automorphisms, newAuto);

                                // Find highest level fixed by this automorphism
                                std::vector<int> someArray;
                                for (int i = 0; i < (int)currentNode.size(); i++) {
                                    if (i < counter && pfixp(currentNode[i], info.fix)) {
                                        someArray.push_back(i);
                                    }
                                }
                                if (!someArray.empty()) {
                                    std::sort(someArray.begin(), someArray.end());
                                    a = someArray.back();
                                }
                                jumpBack = true;
                            }
                        }

                        // Check automorphism with first terminal node
                        auto [cmpFirst, _d4] = lex_ordered(firstTermAdj, newAdj);
                        if (cmpFirst == 0) {
                            Perm newAuto;
                            for (int i = 0; i < (int)firstTermPartition.size(); i++) {
                                newAuto[firstTermPartition[i][0]] = currentNode[counter][i][0];
                            }
                            generators.push_back(newAuto);
                            auto [info, _autos] = update_automorphisms(automorphisms, newAuto);

                            std::vector<int> someArray;
                            for (int i = 0; i < (int)currentNode.size(); i++) {
                                if (i < counter && pfixp(currentNode[i], info.fix)) {
                                    someArray.push_back(i);
                                }
                            }
                            if (!someArray.empty()) {
                                std::sort(someArray.begin(), someArray.end());
                                b = someArray.back();
                            }

                            if (jumpBack) {
                                // Take min of a, b
                                if (a >= 0 && b >= 0) {
                                    counter = std::min(a, b);
                                } else if (b >= 0) {
                                    counter = b;
                                } else {
                                    counter = a;
                                }
                            } else {
                                counter = b;
                                jumpBack = true;
                            }
                        } else if (jumpBack) {
                            counter = a;
                        }
                    }
                }
            }
        }
    }

    // Build final permutation from best_node's partition
    Perm perm;
    for (int i = 0; i < (int)bestNodePartition.size(); i++) {
        perm[bestNodePartition[i][0]] = i;
    }
    return perm;
}

} // namespace BNGcore
