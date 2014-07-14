
package ProcessGraph;
# pragmas
use strict;
use warnings;
no warnings 'redefine';

# Perl Modules
use Class::Struct;
use List::Util qw(min max sum);
#use List::MoreUtils qw( uniq);
use Data::Dumper;

# BNG modules
use StructureGraph;
use BipartiteGraph;
sub uniq { return BipartiteGraph::uniq(@_); }
sub listHas { return BipartiteGraph::listHas(@_); }
sub isSyn { return BipartiteGraph::isSyn(@_); }
sub isDel { return BipartiteGraph::isDel(@_); }
sub any { my @x= @_; return (grep {$_ > 0} @x) ? 1:0 ; }
sub listoverlaps
{
	my @list;
	foreach my $list1(@_) { push @list,@$list1;}
	return 1 if (scalar @list > scalar uniq @list);
	return 0;
}

sub getPatterns
{
	my @edges = @{shift @_};
	my $tr = shift @_;
	my $type = quotemeta shift @_;
	my @patts = 	map {$_ =~ /^.*:(.*):.*$/; $1;}
					grep /$type/,
					grep index($_,$tr)==0, @edges;
	return @patts;
}

sub overlaps
{
	my $edges = shift @_; # an array
	my $tr1 = shift @_;
	my $tr2 = shift @_;
	my $type1 = shift @_;
	my $type2 = shift @_;
	my @patts1 = getPatterns($edges,$tr1,$type1);
	my @patts2 = getPatterns($edges,$tr2,$type2);
	return listoverlaps(\@patts1,\@patts2);
}

sub analyzeGroups
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	my @groups = @{$bpg->{'NodeGroups'}};
	# extend groups to include unassigned nodes
	my @assigned;
	foreach my $grp (@groups) { push @assigned, @$grp; }
	my @unassigned = grep !listHas(\@assigned,$_), @nodelist;
	foreach my $patt (@unassigned)
		{
			my @temp = ( $patt );
			push @groups, \@temp;
		}
	#print map join(" ", @{$_})."\n", @groups;
	
	# extract context
	my @context = grep( /Context/, @edgelist);
	my @syndel = grep( /Syndel/, @edgelist);
	my @cotransforms = grep ( /Cotransform/, @edgelist);

	my @syn = grep( (index($_, '->') == 0), @syndel);
	my @del = grep( (index($_, '->') != 0), @syndel);
	my @uni = ();
	my @bi = ();
	my @all = ();
	
	foreach my $i(0..@groups-1)
	{
		foreach my $j($i..@groups-1)
		{
			my @group1 = @{$groups[$i]};
			my @group2 = @{$groups[$j]};
			my $f = 0;
			my $r = 0;
			foreach my $p (@group1)
			{
				foreach my $q(@group2)
				{
					next if ($p eq $q); 
					# see if $p is a transformation and $q is a pattern
					if ((index($p, '->') != -1) and (index($q, '->') == -1) )
					{
						my $check = $p.":".$q.":";
						my @reverse = grep ( index($_, $check) != -1, (@context,@syn,@del) );
						my @forward = grep( index($_, $check) != -1, @del);
						if (@forward > 0) {$f++;}
						if (@reverse > 0) {$r++;}
					}
					# see if $q is a transformation and $p is a pattern
					if ((index($p, '->') == -1) and (index($q, '->') != -1) )
					{
						my $check = $q.":".$p.":";
						my @forward = grep ( index($_, $check) != -1, (@context,@syn,@del) );
						my @reverse = grep( index($_, $check) != -1, @del);
						if (@forward > 0) {$f++;}
						if (@reverse > 0) {$r++;}
					}
					# see if both $p and $q are transformations
					# check for cotransforms
					if ((index($p, '->') != -1) and (index($q, '->') != -1) )
					{
						my @check = grep index($_, $p) != -1, 
									grep index($_, $q) != -1, 
									@cotransforms;
						if (@check) { $f++; $r++; }
					}
				}
			}
			my $groupname1 = BipartiteGraph::groupName(\@group1);
			my $groupname2 = BipartiteGraph::groupName(\@group2);
			my $string1 = $groupname1." ".$groupname2;
			my $string2 = $groupname2." ".$groupname1;
			if ($f > 0 and $r > 0) { push @bi, $string1." ".$f." ".$r; }
			elsif ($f > 0 ) { push @uni, $string1." ".$f; }
			elsif ($r > 0 ) { push @uni, $string2." ".$r; }
		}
		push @all, BipartiteGraph::groupName($groups[$i]);
	}
	return (\@bi,\@uni,\@all);
}
sub printGroups
{
	my $bpg = shift @_;
	my @groups = @{$bpg->{'NodeGroups'}};
	return map(("Group ".BipartiteGraph::groupName($_)."\n".join("\n",@{$_})."\n\n",@groups));
}




sub analyzeGroups2
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	my @groups = @{$bpg->{'NodeGroups'}};
	# extend groups to include unassigned nodes
	my @assigned;
	foreach my $grp (@groups) { push @assigned, @$grp; }
	my @unassigned = grep !listHas(\@assigned,$_), @nodelist;
	foreach my $patt (@unassigned)
		{
			my @temp = ( $patt );
			push @groups, \@temp;
		}
	# now we need to assign a direction to each element of a group
	my @reac = grep( /Reactant/, @edgelist);
	my @prod = grep( /Product/, @edgelist);
	#my @syndel = grep( /Syndel/, @edgelist);
	
	my @sign_arr = ();
	# each group has a corresponding hash %sign that stores
	# the sign of each of its elements
	# @sign_arr is an array of references to those hashes
	
	foreach my $grp (@groups)
	{	
		my %sign;
		my @group = @$grp;
		my $n = scalar @group;
		my $first = shift @group;

		# if it is a singleton group, element is simply a pattern
		# assign +1 and move on
		if ($n==1) { $sign{$first} = +1; push @sign_arr, \%sign; next; }
		# if not a singleton group
		# first element will be a transformation
		# assign +1 unless it is a del transformation
		$sign{$first} = isDel($first) ? -1 : +1;
		# for the remaining transformation.
		# assign -1 unless it is a syn transformation
		my @trs = grep /->/, @group;
		foreach my $tr(@trs) { $sign{$tr} = isSyn($tr) ? +1 :  -1; }
		
		# separate patterns and wildcard patterns
		my @wc = grep /\!\+/, @group;
		my @temp = (@trs,@wc);
		my @patts = grep !listHas(\@temp,$_), @group;
		my @edges = grep index($_,$first.":")==0, @edgelist;
		my $syndelcheck = (isSyn($first)|isDel($first));

		# assigning signage to each pattern
		foreach my $patt(@patts)
		{
			my $str = ":".$patt.":";
			my @edges2 = grep index($_,$str)!=-1, @edges;
			if ($syndelcheck & @edges2) { $sign{$patt} = +1; next; }
			if (grep /Reactant/,@edges2) { $sign{$patt} = -1; next; }
			if (grep /Product/,@edges2) { $sign{$patt} = +1; next; }
		}
		
		# assigning signage to each wildcard within each group
		# note multiple assignments will happen across the groups
		# if the wildcard is clustered to different groups. that's okay!
		foreach my $wildcard(@wc)
		{
			my @matches = 	grep listHas(\@patts,$_),
							map { $_ =~ /.*:(.*):.*/; $1;}
							grep index($_,$wildcard)!=-1,
							grep /Wildcard/,
							@edgelist;
			$sign{$wildcard} = $sign{$matches[0]}; 
		}
		push @sign_arr, \%sign;
		#unshift @group, $first;	
	}
	#foreach my $sign(@sign_arr) { print printHash($sign); }
	# creating group names
	my %groupname;
	@groupname { 0..@groups-1 } = map groupName2($_), @groups;

	# initializing arrays holding positive & negative influences
	my @influences;
	
	# processing context
	my @context = grep( /Context/, @edgelist);
	foreach my $i(0..@groups-1)
	{
		foreach my $j(0..@groups-1)
		{
			my @group1 = @{$groups[$i]};
			my @group2 = @{$groups[$j]};
			my $pos = 0;
			my $neg = 0;
			foreach my $item1(@group1)
			{
				foreach my $item2(@group2)
				{
					#leave if items are equal
					next if ($item1 eq $item2);
					# context from item1->item2 is stored as
					# item2:item1:Context, with item2 usually a transf
					# and item1 usually a context
					my $str = $item2.":".$item1.":";
					my @conts = grep index($_,$str) == 0, @context;
					#
					# leave if there are no contexts
					next if (! @conts) ;
					foreach my $cont(@conts)
					{
						my $sign1 = $sign_arr[$i]{$item1};
						my $sign2 = $sign_arr[$j]{$item2};
						my $sign = $sign1*$sign2;
						if ($sign > 0) {$pos++;}
						if ($sign < 0) {$neg++;}
						if ($i==$j && @conts) {print $i." ".$j." ".$cont." ".$sign."\n";}
						#print join(" ",($cont, $groupname{$i}, $groupname{$j}, $sign1, $sign2, $sign))."\n";
					}
				}
			}
			if ($pos > 0) { push @influences, "+".$i.":".$j; }
			if ($neg > 0) { push @influences, "-".$i.":".$j; }
		}
	}
	# processing syndel context
	my @syndel = grep( /Syndel/, @edgelist);
	my @syn = grep( (index($_, '->') == 0), @syndel);
	my @del = grep( (index($_, '->') != 0), @syndel);
	foreach my $i(0..@groups-1)
	{
		my @group1 = @{$groups[$i]};
		my $syndelcheck = isSyn($group1[0])|isDel($group1[0]);
		next if ( ! $syndelcheck );
		
		my @trs = grep /->/, @group1;
		my @syn1; my @del1;
		foreach my $tr (@trs) 
		{
			push @del1, grep ( index($_,$tr) == 0, @del );
			push @syn1, grep ( index($_,$tr) == 0, @syn );
		}
		
		foreach my $j(0..@groups-1)
		{
			my $c_syn = 0;
			my $c_del = 0;
			my @group2 = @{$groups[$j]};
			foreach my $item(@group2)
			{
				my $str = ":".$item.":";
				if (grep (index($_,$str) != -1, @del1)) { $c_del++; }
				if (grep (index($_,$str) != -1, @syn1)) { $c_syn++; }
			}
			if ($c_del > 0)
			{
				push @influences, "-".$j.":".$i;
				push @influences, "-".$i.":".$j;
			}
			if ($c_syn > 0)
			{
				push @influences, "+".$i.":".$j;
			}
		}
	}
	
	# processing cotransforms
	my @cotransforms = grep ( /Cotransform/, @edgelist);
	foreach my $i(0..@groups-1)
	{	
		foreach my $j($i+1..@groups-1)
		{
			my @trs1 = grep ( /->/, @{$groups[$i]});
			my @trs2 = grep ( /->/, @{$groups[$j]});
			my $pos = 0;
			my $neg = 0;
			my $check = 0;
			foreach my $cotr(@cotransforms)
				{
				my @splits = split ":",$cotr;
				$check = 1 if (listHas(\@trs1,$splits[0]) && listHas(\@trs2,$splits[1]));
				$check = 1 if (listHas(\@trs2,$splits[0]) && listHas(\@trs1,$splits[1]));
				next if ($check==0);
				my $sign1 = listHas(\@trs1,$splits[0]) ? $sign_arr[$i]{$splits[0]} : $sign_arr[$j]{$splits[0]};
				my $sign2 = listHas(\@trs2,$splits[1]) ? $sign_arr[$j]{$splits[1]} : $sign_arr[$i]{$splits[1]};
				my $sign = -1*$sign1*$sign2;
				# note the additional -1 here when processing tr-tr signs to indicate causality
				if ($sign > 0) {$pos++;}
				if ($sign < 0) {$neg++;}
				}
			if ($pos > 0) { push @influences, "+".$i.":".$j; }
			if ($neg > 0) { push @influences, "-".$i.":".$j; }
		}
	}
	@influences = uniq @influences;
	return (\%groupname,\@influences);
}

sub groupName2
{
	my @group = @{shift @_};
	my $first = $group[0];
	if ($first !~ /->/) {return $first;}
	my ($lhs,$rhs) = map {$_=~ s/\s//g; $_;} split('->',BipartiteGraph::prettify($first));
	if ($first =~ /~/) 
		{ 
		my ($mol,$comp) = ($1,$2) if $lhs =~ /^(.*)\((.*)~.*\)/;
		my $s1 = $1 if $lhs =~ /^.*\(.*~(.*)\)/;
		my $s2 = $1 if $rhs =~ /^.*\(.*~(.*)\)/;
		return $mol."(".$comp."~".$s1."->".$s2.")";
		}
	if ($first =~ /\!/)
		{
		if ($lhs =~ /\!/) 
			{
			my ($m1,$m2) = sort {$a cmp $b} split ",",$rhs;
			return $m1.":".$m2."->";
			}
		if ($rhs =~ /\!/) 
			{
			my ($m1,$m2) = sort {$a cmp $b} split ",",$lhs;
			return "->".$m1.":".$m2;
			}
		}
	# has to be a syn/del transform
	my @trs = grep /->/,@group;
	if ( @trs )
		{
		$first =~ s/->//g;
		return "->".$first."->";
		}
	return $first;
}

sub wildcards2ReactantsProducts
{
	my @edgelist = @{shift @_};
	my @wcs = grep /Wildcard/, @edgelist;
	my @new_edges;
	foreach my $wcline(@wcs)
	{
		my ($wc,$patt) =  split(":",$wcline);
		push @new_edges,
			map { $_.":".$wc.":Reactant"; }
			map { $_ =~ /^(.*):.*:.*$/; $1; }
			grep index($_,":".$patt.":")!=-1,
			grep /Reactant/,
			@edgelist;
		push @new_edges,
			map { $_.":".$wc.":Product"; }
			map { $_ =~ /^(.*):.*:.*$/; $1; }
			grep index($_,":".$patt.":")!=-1,
			grep /Product/,
			@edgelist;
		push @new_edges,
			map { $_.":".$wc.":Syndel"; }
			map { $_ =~ /^(.*):.*:.*$/; $1; }
			grep index($_,":".$patt.":")!=-1,
			grep /Syndel/,
			@edgelist;
	}
	push @edgelist,@new_edges;
	return @edgelist;
}

sub pairs
{
	my @nodelist = @{shift @_};
	my @edgelist = @{shift @_};
	
	my @trs = grep BipartiteGraph::isTransformation($_), @nodelist;
	my @pair_edges = grep /ProcessPair/, @edgelist;
	my @pairs;
	my %paired;
	@paired { @trs } = (0) x @trs;
	foreach my $pair(@pair_edges)
	{
		my @splits = split ":",$pair;
		pop @splits;
		# ignore syndels
		#next if (isSyn($splits[0])|isDel($splits[0]));
		push @pairs, \@splits;
		@paired {@splits} = (1) x @splits;
	}
	my @unpaired = grep ! $paired{$_}, @trs;
	return (\@pairs,\@unpaired,\%paired);
}

sub interactions
{
	my @nodelist = @{shift @_};
	my @edgelist = @{shift @_};
	my @trs = grep BipartiteGraph::isTransformation($_), @nodelist;
	
	my @act = ();
	my @inh = ();
	my @syn = ();
	my @del = ();
	my $i = -1;
	foreach my $tr1(@trs)
	{
		$i++;
		my $j = -1;
		foreach my $tr2(@trs)
		{
		$j++;
		if (overlaps(\@edgelist,$tr1,$tr2,'Product','Context')) { push @act, join(":",($tr1,$tr2)); }
		if (isSyn($tr1) and overlaps(\@edgelist,$tr1,$tr2,'Syndel','Context')) { push @syn, join(":",($tr1,$tr2)); }
		if (isDel($tr1) and overlaps(\@edgelist,$tr1,$tr2,'Syndel','Context')) { push @del, join(":",($tr1,$tr2)); }
		#if (isDel($tr1) and overlaps(\@edgelist,$tr1,$tr2,'Syndel','Product')) { push @del, join(":",($tr2,$tr1)); }
		if ($j > $i and overlaps(\@edgelist,$tr1,$tr2,'Reactant','Reactant')) { push @inh, join(":",($tr1,$tr2)); }
		}
	}
	
	# do cotransforms
	my @cotrs = grep /Cotransform/, @edgelist;
	foreach my $cotr(@cotrs)
	{
		my @trs1 = split ":",$cotr;
		pop @trs1;
		#my @trs2 = map BipartiteGraph::reverseTransformation($_), @trs1;
		foreach my $i(0..@trs1-1)
		{
		foreach my $j($i+1..@trs1-1)
			{
			my ($tr1,$tr2) = @trs1;
			my ($tr1r,$tr2r) = map BipartiteGraph::reverseTransformation($_), @trs1;
			if (listHas(\@nodelist,$tr1r)) { push @act, join(":",$tr1r,$tr2); }
			if (listHas(\@nodelist,$tr2r)) { push @act, join(":",$tr2r,$tr1); }
			}
		}
	}
	return (\@act,\@inh,\@syn,\@del);
}

sub compute_influence
{
	my ($node1,$node2) = @{shift @_};
	my @list = uniq @{shift @_};
	my @trs1 = @$node1;
	my @trs2 = @$node2;
	
	my ($hash1,$hash2) = @{shift @_};
	my %singlehash = %$hash1;
	my %pairhash = %$hash2;
	
	# check if influence exists, else return
	my @tuples = ();
	foreach my $tr1(@trs1) { foreach my $tr2(@trs2) { push @tuples, join(":",($tr1,$tr2)); }}
	my @infs = map {my $x = $_; scalar grep {$_ eq $x} @list} @tuples;
	if (sum(@infs)==0) { return 0; }
	
	# now we need to figure out how many influences COULD exist
	my ($num1,$num2) = map scalar(@$_),($node1,$node2);
	
	# now what are these influences
	my @list2 = ();
	foreach my $x(@tuples) { my @y = grep {$_ eq $x} @list; if (@y) { push @list2,@y; } }
	my @tos = uniq map { $_ =~ /.*:(.*)/; $1; } @list2;
	my @froms = uniq map { $_ =~ /(.*):.*/; $1; } @list2;
	my ($n_to,$n_from) = map scalar(@$_),(\@tos,\@froms);
	
	# num1 = 1,2
	# num2 = 1,2

	my $source = $singlehash{$froms[0]};
	my $target = $singlehash{$tos[0]};
	my $type = 'Activation';
	
	if($num1==1 and $num2==1) { }
	elsif($num1==1 and $num2==2)
		{
		$target = $pairhash{$tos[0]} if (sum(@infs) ==2 );
		}
	elsif($num1==2 and $num2==1)
		{
		$source = $pairhash{$froms[0]} if (sum(@infs) ==2 );
		}
	elsif (sum(@infs) > 2)
		{
		$source = $pairhash{$froms[0]} ;
		$target = $pairhash{$tos[0]};
		}
	elsif (sum(@infs) == 2)
		{
		if ( scalar(@froms) == scalar (@tos) )
			{
			$source = $pairhash{$froms[0]} ;
			$target = $pairhash{$tos[0]};
			# (F1,R1), (F2,R2), could be F1->F2, R1->R2 or F1->R2,R1->F2
			$type = 'Activation' if (join("",@infs) eq "1001") ;
			$type = 'Inhibition' if (join("",@infs) eq "0110") ;
			}
		else
			{
			$source = $pairhash{$froms[0]} if (scalar (@froms) ==2);
			$target = $pairhash{$tos[0]} if (scalar (@tos) ==2);
			}
		}
	return join(":",($source,$target,$type));
	
}

sub influences
{
	my @nodes = @{shift @_};
	my @ints = @{shift @_};
	my @act = @{$ints[0]};
	my @inh = @{$ints[1]};
	my @syn = @{$ints[2]};
	my @del = @{$ints[3]};
	my %paired = %{shift @_};
	#print BipartiteGraph::printHash(\%paired);
	
	# pair each tr to the index of its group
	# pair each tr to its own index
	my %pairhash;
	my %singlehash;
	foreach my $i(0..@nodes-1)
	{
		my @trs = @{$nodes[$i]};
		if (scalar @trs == 2) { @pairhash { @trs } = ($i) x @trs; }
		if (scalar @trs == 1) { @singlehash { @trs } = ($i) x @trs; }
	}
	
	my @influences = ();
	my $i=-1;
	foreach my $node1(@nodes)
	{	
		$i++; my $j=-1;
		foreach my $node2(@nodes)
		{
			$j++;
			my @trs1 = @$node1;
			my @trs2 = @$node2;
			my $isgroup1 = (scalar @trs1 > 1) ? 1 : 0;
			my $isgroup2 = (scalar @trs2 > 1) ? 1 : 0;
			my $ingroup1 = ($paired{$trs1[0]}) ? 1 : 0;
			my $ingroup2 = ($paired{$trs2[0]}) ? 1 : 0;
			next if ( ($isgroup1 != $ingroup1) or ($isgroup2 != $ingroup2) );
			# 1x1x, 0x0x, x1x1, x0x0 are the only allowed configurations
			
			my $inf = compute_influence([$node1,$node2],\@act,[\%singlehash,\%pairhash]);
			if ($inf) { push @influences,$inf;}
		}
	}
	
	
	
	foreach my $inf(@inh)
	{
		push @influences, join(":",( @singlehash { split(":",$inf) }, 'Competition' ));
	}
	
	# get synthesis processes
	my @syn_trs = uniq map {$_ =~ /(.*):.*/; $1; } @syn;
	my @del_trs = uniq map {$_ =~ /(.*):.*/; $1; } @del;	
	
	
	
	foreach my $tr(@syn_trs)
	{
		my $node1 = [$tr];
		foreach my $node2(@nodes)
		{
			my @trs2 = @$node2;
			my $isgroup2 = (scalar @trs2 > 1) ? 1 : 0;
			my $ingroup2 = ($paired{$trs2[0]}) ? 1 : 0;
			next if ($isgroup2 != $ingroup2) ;
			my $inf = compute_influence([$node1,$node2],\@syn,[\%singlehash,\%pairhash]);
			if ($inf)
				{
				$inf =~ s/Activation/Synthesis/;
				push @influences,$inf;
				}
		}
	}
	
	foreach my $tr(@del_trs)
	{
		my $node1 = [$tr];
		foreach my $node2(@nodes)
		{
			my $isgroup2 = (scalar @$node2 > 1) ? 1 : 0;
			my $ingroup2 = ($paired{$$node2[0]}) ? 1 : 0;
			next if ($isgroup2 != $ingroup2) ;
			my $inf = compute_influence([$node1,$node2],\@del,[\%singlehash,\%pairhash]);
			if ($inf)
				{
				$inf =~ s/Activation/Degradation/;
				push @influences,$inf;
				}
		}
	}
	
	return uniq @influences;
}

sub makeProcessGraph
{
	my $bpg = shift @_;
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	
	
	# getting transformations, pairs, syndels
	my @trs = grep BipartiteGraph::isTransformation($_), @nodelist;
	my ($x,$y,$z) = pairs(\@nodelist,\@edgelist);
	my @pairs = @$x;
	my @unpaired = @$y;
	my %paired = %$z;
	my @syndels = grep isSyn($_)|isDel($_), @nodelist;
	
	# making nodes
	my @nodes;
	push @nodes, @pairs;
	foreach my $tr(@trs) { my @x = ($tr); push @nodes,\@x; }
	# nodes are either groups or transformations.
	# all transformations are separate nodes, including those in groups
	# each node is an array,
	# e.g., if A->B, B->A, B->C exists, then the nodes would be
	# @nodes = ( [A->B,B->A], [A->B], [B->A]),[B->C]), [] being an arrayref
	
	# we need to extend wildcard relations to reactant/product relations
	# i.e. if A(a!0).B(b!0) is a reactant, then A(a!+) is also a reactant
	my @wcs = grep /Wildcard/, @edgelist;
	@edgelist = wildcards2ReactantsProducts(\@edgelist);

	# compiling activating and inhibiting relationships between transformations
	my @ints = interactions(\@nodelist,\@edgelist);

	# recompiling influences by merging common edges of groups
	my @influences = influences(\@nodes,\@ints,\%paired);
	
	return (\@nodes,\@influences);
	
}


sub makeProcessGraph2
{
	my $bpg = shift @_;
	#print BipartiteGraph::printGraph($bpg);
	my @nodelist = @{$bpg->{'NodeList'}};
	my @edgelist = @{$bpg->{'EdgeList'}};
	
	# getting transformations and initializing indexes
	my @trs = grep BipartiteGraph::isTransformation($_), @nodelist;
	#print map $_."\n",@trs;
	
	# first, we need to extend wildcard relations to reactant/product relations
	# i.e. if A(a!0).B(b!0) is a reactant, then A(a!+) is also a reactant
	my @wcs = grep /Wildcard/, @edgelist;
	my @new_edges;
	foreach my $wcline(@wcs)
	{
		my ($wc,$patt) =  split(":",$wcline);
		push @new_edges,
			map { $_.":".$wc.":Reactant"; }
			map {$_ =~ /^(.*):.*:.*$/; $1;}
			grep index($_,":".$patt.":")!=-1,
			grep /Reactant/,
			@edgelist;
		push @new_edges,
			map { $_.":".$wc.":Product"; }
			map {$_ =~ /^(.*):.*:.*$/; $1;}
			grep index($_,":".$patt.":")!=-1,
			grep /Product/,
			@edgelist;
	}
	push @edgelist,@new_edges;
	
	# now we need to map relationships between processes
	# initializing influences array
	# tr1:tr2:Influence
	# influence can be Activation, Deactivation, Pair, Competition, SharedComponent
	my @influences;
	foreach my $i(0..@trs-1)
		{
		foreach my $j($i..@trs-1)
			{
			my $tr1 = $trs[$i];
			my $tr2 = $trs[$j];

			# activation
			if (overlaps(\@edgelist,$tr1,$tr2,'Product','Context'))
				{push @influences, join(":",($tr1,$tr2,'Activation'));}
			if (overlaps(\@edgelist,$tr2,$tr1,'Product','Context'))
				{push @influences, join(":",($tr2,$tr1,'Activation'));}
			
			# syndel
			if (overlaps(\@edgelist,$tr1,$tr2,'Syndel','Context'))
				{
				if (isSyn($tr1))
					{push @influences, join(":",($tr1,$tr2,'Synthesis'));}
				if (isDel($tr1))
					{push @influences, join(":",($tr1,$tr2,'Degradation'));}
				}
			if (overlaps(\@edgelist,$tr2,$tr1,'Syndel','Context'))
				{
				if (isSyn($tr2))
					{push @influences, join(":",($tr2,$tr1,'Synthesis'));}
				if (isDel($tr2))
					{push @influences, join(":",($tr2,$tr1,'Degradation'));}
				}
				
				
			# inhibition
			if($j > $i)
			{
				if (overlaps(\@edgelist,$tr1,$tr2,'Reactant','Reactant'))
				{
				push @influences, join(":",($tr1,$tr2,'Competition'));
				push @influences, join(":",($tr2,$tr1,'Competition'));
				}
			}
			# deactivation
			#if (overlaps(\@edgelist,$tr1,$tr2,'Reactant','Context'))
			#	{push @influences, join(":",($tr1,$tr2,'Deactivation'));}
			#if (overlaps(\@edgelist,$tr2,$tr1,'Reactant','Context'))
			#	{push @influences, join(":",($tr2,$tr1,'Deactivation'));}
			
			# pair
			if ($tr1 eq BipartiteGraph::reverseTransformation($tr2))
				{
				push @influences, join(":",($tr1,$tr2,'Pair'));
				push @influences, join(":",($tr2,$tr1,'Pair'));
				}
			
			}
		}
		
	@influences = uniq @influences;
	#print map $_."\n",@influences;
	return (\@trs,\@influences);
	

}




1;