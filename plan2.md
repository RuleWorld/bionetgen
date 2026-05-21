Let's see if we can understand what "TODO: HANDLE defined $node->{'embed'} see line 942" really means.
Line 942 is inside `printGML`:
```perl
		my $string3 = '';
		if(defined $node->{'embed'})
		{
			my $somegraph = $node->{'embed'};
			my $type = ref $somegraph;
			if($type eq 'StructureGraph')
			{
				if($somegraph->{'Type'} eq 'Rule')
					{$string3 = toGML_rule_operation($somegraph, "gml");}
				else
					{$string3 = toGML_pattern($somegraph, "gml");}
			}
			elsif($type eq 'NetworkGraph')
			{ $string3 = toGML_rule_network($somegraph, "gml");}
		}
```
Wait! I added `"gml"` above in my head, but the original code is:
```perl
				if($somegraph->{'Type'} eq 'Rule')
					{$string3 = toGML_rule_operation($somegraph);}
				else
					{$string3 = toGML_pattern($somegraph);}
```
If we don't pass `$outType`, it defaults to `"graphml"`.
Wait, in `printGML`, the default is "graphml"? Let's look at `toGML_pattern`:
`my $outType = @_ ? shift @_: "graphml";`
So if `printGML` calls `toGML_pattern($somegraph)`, it will return `"graphml"` format!
Wait, `printGML` calls it without specifying `$outType`, so `$string3` in `printGML` gets GraphML XML inside the GML output?!
Let's verify this.
