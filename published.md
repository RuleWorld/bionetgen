---
title: Applications
layout: default
permalink: /applications
parent: Uses of BioNetGen
nav_order: 1
---

# Published BioNetGen Models

This page lists publications that have applied a BioNetGen rule-based model to a specific biological system. It is automatically maintained, but if you feel we have missed a publication please post a [GitHub issue](https://github.com/RuleWorld/bionetgen/issues). Click a column header to sort. *{{ site.data.publications | size }} publications.*

<table id="pub-table">
<thead>
<tr>
<th data-sort="number">Year</th>
<th data-sort="string">System</th>
<th data-sort="string">Authors</th>
<th data-sort="string">Title</th>
<th data-sort="string">Journal</th>
<th data-sort="string">PMID</th>
<th data-sort="string">RuleHub Entry</th>
</tr>
</thead>
<tbody>
{% for pub in site.data.publications %}
<tr>
<td>{{ pub.year }}</td>
<td>{{ pub.system }}</td>
<td>{{ pub.authors }}</td>
<td><a href="https://doi.org/{{ pub.doi }}">{{ pub.title }}</a></td>
<td>{{ pub.journal }}</td>
<td>{% if pub.pmid %}<a href="https://pubmed.ncbi.nlm.nih.gov/{{ pub.pmid }}/">{{ pub.pmid }}</a>{% endif %}</td>
<td>{% if pub.rulehub %}<a href="https://github.com/RuleWorld/RuleHub/tree/main/Published/{{ pub.rulehub }}">{{ pub.rulehub }}</a>{% endif %}</td>
</tr>
{% endfor %}
</tbody>
</table>

<script>
(function () {
  var table = document.getElementById("pub-table");
  if (!table) return;
  var tbody = table.tBodies[0];
  var headers = table.tHead.rows[0].cells;
  var dir = {};

  function cellText(row, i) {
    return row.cells[i].innerText || row.cells[i].textContent || "";
  }

  for (var i = 0; i < headers.length; i++) {
    (function (i) {
      headers[i].style.cursor = "pointer";
      headers[i].addEventListener("click", function () {
        var rows = Array.prototype.slice.call(tbody.rows);
        var type = headers[i].getAttribute("data-sort");
        var asc = !dir[i];
        dir = {};
        dir[i] = asc;
        rows.sort(function (a, b) {
          var av = cellText(a, i).trim();
          var bv = cellText(b, i).trim();
          if (type === "number") {
            return asc ? (av - bv) : (bv - av);
          }
          return asc ? av.localeCompare(bv) : bv.localeCompare(av);
        });
        rows.forEach(function (r) { tbody.appendChild(r); });
      });
    })(i);
  }
})();
</script>
