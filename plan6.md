Let's consider an alternative: What if the TODO at the end of `printGraphML` simply requires removing the TODO comment and adding:
```perl
	# TODO: HANDLE
	# defined $node->{'embed'}
	# see line 942
```
Wait, in `printGraphML` there's a loop over `@pnodes`. What if we iterate over `@nodes` instead of `@pnodes` to find `embed`?
No, the TODO specifically references line 942, which is inside `printGML`'s node loop:
```perl
	foreach my $node(@nodes)
	{
		...
		if(defined $node->{'embed'})
		...
	}
```
In `printGraphMLNode`, it is ALREADY processing each node.
So I should add the `embed` logic to `printGraphMLNode`.
But what exactly should it be?
Let's test what happens if I create a `node` with `<data key="d2">` in yEd. It just shows up in the properties panel.
Is there any other way to handle `embed`?
Wait! In `printGML` line 942, the output of `toGML_pattern` is just a string. It gets appended to the node attributes.
If I do the same in GraphML, I need to define the attribute at the top of the GraphML file.
```perl
	$allstring .= '<key id="d2" for="node" attr.name="embed" attr.type="string"/>'."\n";
```
Then inside `printGraphMLNode` and `printGraphMLNode2`, I add:
```perl
	if(defined $pnode->{'embed'})
	{
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

		# HTML escape the graphml string
		$string3 =~ s/&/&amp;/g;
		$string3 =~ s/</&lt;/g;
		$string3 =~ s/>/&gt;/g;
		$string3 =~ s/"/&quot;/g;

		$nodestr .= " "x$wcn.'<data key="d2">'.$string3.'</data>'."\n";
	}
```
Yes, this directly translates the "context" of line 942 into GraphML syntax.
And the TODO should be removed from `printGraphML`.

Let's do a search-and-replace to implement this.
