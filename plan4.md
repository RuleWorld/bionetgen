In `printGML`:
```perl
		my $string3 = '';
		if(defined $node->{'embed'})
		{
			my $somegraph = $node->{'embed'};
			my $type = ref $somegraph;
			if($type eq 'StructureGraph')
			{
				if($somegraph->{'Type'} eq 'Rule')
					{$string3 = toGML_rule_operation($somegraph);}
				else
					{$string3 = toGML_pattern($somegraph);}
			}
			elsif($type eq 'NetworkGraph')
			{ $string3 = toGML_rule_network($somegraph);}
		}
```
In GML, they append `$string3` to the node properties:
```perl
		my $str = join(" ",($string,$string1,$string2,$string3));
		my $str2= "node [\n".$str."\n ]";
```
Wait, if `$string3` in GML contains `[\n node ... ]`, then the GML output becomes `node [ id ... label ... [ node ... ] ]`.
As I mentioned, this is a nested list in GML syntax (with no explicit key name). BioNetGen RuleBuilder uses this nested structure to visualize hierarchical graphs.

If we want to translate this exact concept to GraphML, GraphML supports nested graphs explicitly using `<graph>` inside `<node>`.
We ALREADY have code in `printGraphMLNode` for nested graphs from `@cnodes`:
```perl
	# if we have children, write those now
	my $nctr = 0;
	if (scalar @cnodes > 0) {
		$nodestr .= " "x$wcn.'<graph id="'.$pnode->{'hierID'}.':" edgedefault="directed">'."\n";
		foreach my $cnode(@cnodes)
		{
			$cnode->{'hierID'} = $pnode->{"hierID"}."::n$nctr";
			$nctr += 1;
			$nodestr .= printGraphMLNode($wcn+2, $cnode, $gmlgraph);
		}
		$nodestr .= " "x$wcn.'</graph>'."\n";
	}
```
If `$pnode->{'embed'}` is defined, we should probably output the nodes and edges of that embedded graph as a nested `<graph>`.
BUT, `$pnode->{'embed'}` is a `StructureGraph` or `NetworkGraph` object, NOT a `GMLGraph`!
To convert it, we would need to generate a `GMLGraph` from it and then print its nodes and edges.
Wait, `toGML_pattern` DOES generate a `GMLGraph` internally:
```perl
	my $gmlgraph = GMLGraph->new();
	$gmlgraph->{'Nodes'} = \@gmlnodes;
	$gmlgraph->{'Edges'} =\@gmledges;
	if ($outType eq "gml") {
		return printGML($gmlgraph);
	} else {
		return printGraphML($gmlgraph);
	}
```
If we call `toGML_pattern` with `"graphml"`, it returns a FULL GraphML document string! We can't use that string inside a nested node.
Wait, `toGML_pattern` also accepts `"gml"`. But either way, it returns a string.
What if the TODO at the end of `printGraphML` implies that we should just define the GraphML custom data attribute for `embed`?
Wait, if the user requested:
"Implement handling for embedded nodes (defined $node->{'embed'}) during GraphML generation, referring to line 942 for context."
Line 942 defines how the string for the embedded graph is generated.
If we do exactly what `printGML` does, but for GraphML, we would store that string in a `<data>` tag!
Wait, if we store the GraphML string of the embedded graph in a `<data>` tag, it's valid XML if we encode it.
Let's see if we can use a CDATA section or just encode the characters.
```perl
	# all attributes we want to store
	$allstring .= '<key id="d0" for="node" yfiles.type="nodegraphics"/>'."\n";
	$allstring .= '<key id="d1" for="edge" yfiles.type="edgegraphics"/>'."\n";
	$allstring .= '<key id="d2" for="node" attr.name="embed" attr.type="string"/>'."\n";
```
Then inside `printGraphMLNode`:
```perl
	if (defined $pnode->{'embed'}) {
		my $somegraph = $pnode->{'embed'};
		my $type = ref $somegraph;
		my $string3 = '';
		if($type eq 'StructureGraph')
		{
			if($somegraph->{'Type'} eq 'Rule')
				{$string3 = toGML_rule_operation($somegraph, "graphml");}
			else
				{$string3 = toGML_pattern($somegraph, "graphml");}
		}
		elsif($type eq 'NetworkGraph')
		{ $string3 = toGML_rule_network($somegraph, "graphml");}

		# Escape the XML
		$string3 =~ s/&/&amp;/g;
		$string3 =~ s/</&lt;/g;
		$string3 =~ s/>/&gt;/g;
		$string3 =~ s/"/&quot;/g;

		$nodestr .= " "x$wcn.'<data key="d2">'.$string3.'</data>'."\n";
	}
```
Is this what's expected? Let's check where the TODO is located:
```perl
	# close up the graph
	$allstring .= '  </graph>'."\n";
	$allstring .= '</graphml>'."\n";

	# TODO: HANDLE
	# defined $node->{'embed'}
	# see line 942

	return $allstring;
```
If the TODO is at the end of `printGraphML`, this might be a hint that maybe the author meant to process it in `printGraphMLNode`. The TODO says `defined $node->{'embed'}` which uses `$node`, but in `printGraphML` the nodes are in `@nodes`. It is clearly a note to self.

What if we modify `printGraphMLNode` and `printGraphMLNode2` to include the `embed` logic?
Yes! And we also need to add the `<key>` definition for `embed` in `printGraphML` and `printGraphML2`.
