parser grammar BNGGrammar_Observables;

options {
  language = Java;
  output = template;
 
}
@members{
 public void getParentTemplate(){
 
  this.setTemplateLib(gParent.getTemplateLib());

 }
     @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    return gParent.getErrorMessage(e,tokenNames);
  }
}

observables_block[List observables]
scope
{
  int observableCounter;
  ReactionAction reactionAction;
}
@init{
getParentTemplate();
gParent.paraphrases.push("in the observables section");
  $observables_block::observableCounter = 1;
  $observables_block::reactionAction = new ReactionAction();
}
@after{
  gParent.paraphrases.pop();
}
        : BEGIN OBSERVABLES LB+(s1=observable_def_line["O" + $observables_block::observableCounter] 
             {observables.add(s1.st);$observables_block::observableCounter++;} LB+)* END OBSERVABLES LB*
        ;
observable_def_line[String upperID]
scope{
  List<StringTemplate> pattern;

}
@init{
  $observable_def_line::pattern = new ArrayList<StringTemplate>();

}
        : 
        ({gParent.netGrammar}? INT | )
        (s1=observable_type)? STRING {gParent.memory.put($STRING.text,new Register(0.0,"observable"));} 
        ({gParent.netGrammar}? LBRACKET MATCHONCE RBRACKET | ) (pattern_list[upperID] { $observable_def_line::pattern.add($pattern_list.st);})
        -> observables_block(id={upperID},type={$observable_type.text},patterns={$observable_def_line::pattern},name={$STRING.text})
        ;
observable_type
        : (MOLECULES) | (SPECIES)
        ;
        
pattern_list[String upperID]
scope
{
  List patterns;
  int patternCounter;
  List<String> relationships;
  List<String> quantity;
}
@init{
  $pattern_list::patterns = new ArrayList();
  $pattern_list::patternCounter = 1;
  $pattern_list::relationships = new ArrayList<String>();
  $pattern_list::quantity = new ArrayList<String>();
   

  // ((ChangeableChannelTokenStream)input).addChannel(0);
}

        : (s1=obs_species_def[upperID + "_P" + $pattern_list::patternCounter, $observables_block::reactionAction] 
        {
	        $pattern_list::patterns.add(s1.st);
	        $pattern_list::patternCounter++;
	        if($s1.relationship.equals("")){
	          $pattern_list::relationships.add("");
	          $pattern_list::quantity.add("");
	        }
	        else{
	          $pattern_list::relationships.add($s1.relationship);
            $pattern_list::quantity.add($s1.quantity);
	        }
	        
	        
        }
        ((COMMA)? s2=obs_species_def[upperID + "_P" + $pattern_list::patternCounter, $observables_block::reactionAction] 
        {
	        $pattern_list::patterns.add(s2.st);
	        $pattern_list::patternCounter++; 
	        if($s2.relationship.equals("")){
            $pattern_list::relationships.add("");
            $pattern_list::quantity.add("");
          }
          else{
            $pattern_list::relationships.add($s2.relationship);
            $pattern_list::quantity.add($s2.quantity);
            
          }
        })*)
       -> patterns_block(id={upperID + "_P" + $pattern_list::patternCounter},molecules={$pattern_list::patterns},
       relationship={$pattern_list::relationships},quantity={$pattern_list::quantity})
        ;

//similar to rules_species_def, but it doesn't accept stoichiometry factors or a zero
 obs_species_def[String upperID,ReactionAction reactionAction] returns [int stoichiometry,Map <String,List<ReactionRegister>> map, 
                        String relationship = "", String quantity = ""]
scope{
List reactants;
BondList bonds;
}
@init{
  $obs_species_def::reactants = new ArrayList();
  $obs_species_def::bonds = new BondList();
  $stoichiometry = 1;
}
: 
 
 s1=(species_def[$obs_species_def::reactants,$obs_species_def::bonds,upperID] 
  {
       reactionAction.addMolecule(upperID,$species_def.text,$obs_species_def::bonds);
       $map = $species_def.listOfMolecules;
  }) (relational_observer
  {
    $relationship = $relational_observer.relationship;
    $quantity = $relational_observer.quantity;
  }
  
  )?
 
    
    ->rule_seed_species_block(id={upperID},molecules={$obs_species_def::reactants},firstBonds={$obs_species_def::bonds.getLeft()},
      secondBonds={$obs_species_def::bonds.getRight()})
    ;

relational_observer returns [String relationship, String quantity]
:
s1 = (EQUALS | GT | GTE | LT| LTE)
{
$relationship = $s1.text;
}
 s2 = (STRING | INT)
 {
 $quantity = $s2.text;
 }
;
