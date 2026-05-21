Wait, `toGML_pattern` takes multiple arguments. Let's look at it.
If we want to implement `embed` in GraphML, the TODO is in `printGraphML`, which handles the GraphML (XML) generation.
Wait, if the TODO is:
```perl
	# TODO: HANDLE
	# defined $node->{'embed'}
	# see line 942
```
And line 942 is inside `printGML` where it iterates through all `@nodes` and appends the embedded graph's GML output into the node's properties.
In `printGraphML`, we iterate through `@nodes`, but where do we do it? We don't. We only iterate through `@pnodes` (nodes with no `gid`).
```perl
	my $wc = 4;
	my @pnodes = grep { $_->{'gid'} eq '' } @nodes;
	my $nctr = 0;
	foreach my $pnode(@pnodes)
	{
		$pnode->{'hierID'} = "n$nctr";
		$nctr += 1;
		$allstring .= printGraphMLNode($wc, $pnode, $gmlgraph);
	}
```
If we want to handle `embed` in GraphML, we should probably output the embedded graph inside the `<node>` tag, just like we do for children (`@cnodes`).
BUT `printGraphMLNode` is what generates the `<node>` tag!
Why is the `TODO` at the END of `printGraphML`?
Ah! If the `TODO` is at the end of `printGraphML`, maybe the author meant "we also need to process `$node->{'embed'}`" and left the comment at the bottom of the function.
Wait, `printGraphML` is GraphML format 1. Is there a GraphML format 2? Yes, `printGraphML2`.
`printGraphML2` does the same iteration over `@pnodes`, calls `printGraphMLNode2`, but has NO TODO at the end.
Let's see if `printGraphMLNode` handles `embed`. No.
Let's see if `printGraphMLNode2` handles `embed`. No.

If we want to handle `embed` in GraphML, how should we do it?
In `printGML` (GML format), it just calls `toGML_pattern($somegraph)`, which internally calls `printGraphML` or `printGML` depending on `$outType`.
Wait! In `printGML` (line 942):
```perl
			if($type eq 'StructureGraph')
			{
				if($somegraph->{'Type'} eq 'Rule')
					{$string3 = toGML_rule_operation($somegraph);}
				else
					{$string3 = toGML_pattern($somegraph);}
			}
			...
```
Wait, `toGML_pattern($somegraph)` without second argument will default to `graphml`!
This means `printGML` is putting GraphML XML output inside the GML output if it doesn't specify `"gml"` as the second argument! That seems like a bug in `printGML` introduced when `$outType` was added ("AS2021 - adding output types").
Let's check when `$outType` was added.
`my $outType = @_ ? shift @_: "graphml"; # the output type`
If it defaults to `"graphml"`, then `toGML_pattern($somegraph)` returns GraphML XML!

But let's focus on our task: "Implement handling for embedded nodes (defined $node->{'embed'}) during GraphML generation, referring to line 942 for context."
In `printGraphMLNode` (and `printGraphMLNode2`?), if a node has `embed`, we should call the appropriate `toGML_...($somegraph, "graphml")` function.
BUT wait, `toGML_...($somegraph, "graphml")` returns a fully-formed GraphML document (with `<?xml ...>` and `<graphml ...>` tags).
We can't just embed a full XML document inside `<node>`.
What if we use `<data key="d2">` to store it?
YEd supports `y:GraphMLReference` or we can just ignore yEd and store it as plain text in a `<data>` attribute for another parser to use?
Wait, if `embed` is a graph, we could represent it as a nested `<graph>` in GraphML!
Just like this:
```perl
	if (scalar @cnodes > 0) {
		$nodestr .= " "x$wcn.'<graph id="'.$pnode->{'hierID'}.':" edgedefault="directed">'."\n";
        ...
		$nodestr .= " "x$wcn.'</graph>'."\n";
	}
```
But `toGML_pattern($somegraph, "graphml")` returns the whole document!
If we want to just embed the inner nodes and edges, we would need a function that just returns the inner nodes and edges.
Wait, `toGML_pattern` actually constructs a `GMLGraph` and calls `printGraphML2($gmlgraph)`.
If we just take the `$somegraph` and convert it into nodes and edges, we could recursively call `printGraphMLNode`!
Wait! We could just do:
```perl
		my $string3 = '';
		if(defined $pnode->{'embed'})
		{
			my $somegraph = $pnode->{'embed'};
			my $type = ref $somegraph;
			if($type eq 'StructureGraph')
			{
				if($somegraph->{'Type'} eq 'Rule')
					{$string3 = toGML_rule_operation($somegraph, "graphml");}
				else
					{$string3 = toGML_pattern($somegraph, "graphml");}
			}
			elsif($type eq 'NetworkGraph')
			{ $string3 = toGML_rule_network($somegraph, "graphml");}
		}
```
If we do this, `$string3` contains a full XML document.
How can a full XML document be embedded in GraphML?
GraphML supports embedding other XML using `<data>` tags, provided they are CDATA or the schema allows it, or it could be placed in a generic `<data key="embed">` as escaped text.
Wait, let's look at how yEd does it. If we have a nested graph, it just puts it in `<graph>`. But `embed` is used for things like patterns inside a rule node, right?
Wait! In GML (line 960), it just appends it to the node properties:
`my $str = join(" ",($string,$string1,$string2,$string3));`
If `string3` is a full XML document (due to the `graphml` default), appending it to GML properties is definitely a bug in `printGML`! BUT if the user asked to "Implement handling for embedded nodes... during GraphML generation", let's look at `printGraphML` (around line 652):
```perl
	# close up the graph
	$allstring .= '  </graph>'."\n";
	$allstring .= '</graphml>'."\n";

	# TODO: HANDLE
	# defined $node->{'embed'}
	# see line 942

	return $allstring;
```
If we put the handler AT THE END of `printGraphML`, how does it modify the nodes? It's too late! The nodes have already been printed into `$allstring`.
The author probably just dropped the TODO at the end of the file/function. We need to implement it where nodes are processed.
Where are nodes processed in `printGraphML`?
`printGraphML` calls `printGraphMLNode`.
So we should probably add it inside `printGraphMLNode` and `printGraphMLNode2`.
Wait! Look at `printGraphMLNode` (line 740):
```perl
	# done with parent node
	$nodestr .= " "x$wc.'</node>'."\n";
	return $nodestr;
```
If we want to append the embedded graph, we could put it inside the `<node>`.
But what should the XML be?
If we look at `printGraphML`, it defines keys:
```perl
	$allstring .= '<key id="d0" for="node" yfiles.type="nodegraphics"/>'."\n";
	$allstring .= '<key id="d1" for="edge" yfiles.type="edgegraphics"/>'."\n";
```
There is NO key for `embed`.
If we define a key for `embed`:
```perl
	$allstring .= '<key id="d2" for="node" attr.name="embed" attr.type="string"/>'."\n";
```
Then inside the node, we can do:
```perl
	if(defined $pnode->{'embed'}) {
		# ... get $string3 ...
		# encode $string3 for XML
		$string3 =~ s/&/&amp;/g;
		$string3 =~ s/</&lt;/g;
		$string3 =~ s/>/&gt;/g;
		$string3 =~ s/"/&quot;/g;
		$nodestr .= " "x$wcn.'<data key="d2">'.$string3.'</data>'."\n";
	}
```
Is this what is expected?
Let's see what happens if we just use the nested `<graph>` element.
If we use `<graph>`, yEd will show it as a group node containing those nodes!
But wait, `toGML_pattern($somegraph, "graphml")` returns a FULL GraphML document string! We can't insert a full GraphML document inside `<graph>`. It would have `<?xml ...>` etc.
So the author MUST mean to store the document as a string inside a `<data>` tag, or the author expects us to parse it?
No, wait. If we just want to create a group node, we would convert `$somegraph` to a `GMLGraph` and then render its nodes/edges recursively!
BUT `toGML_pattern` takes the `StructureGraph` and returns a string directly.
If we want to return a string, maybe the string is just meant to be attached to the node as a data attribute?
Let's check if there is any mention of `d2` or `embed` key in the file.
No.
What if we add `<data key="d2">`? Or maybe yEd has an `embed` feature?
