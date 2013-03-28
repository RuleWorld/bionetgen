parser grammar BNGGrammar_SeedSpecies;

options {
  language = Java;
  output = template;
 
}
@members{
 public void getParentTemplate(){
 
  this.setTemplateLib(gParent.getTemplateLib());
 }
}

seed_species_block[List seedSpecies]
scope{
 int numSpecies;
 }
@init{
 $seed_species_block::numSpecies = 1;
 //this is meant as information for the error logger. it provides information of which section the parser is currently processing
   getParentTemplate();
   gParent.paraphrases.push("in seed species section");
   
}
@after{
  gParent.paraphrases.pop();
}
: BEGIN (SEED)? SPECIES LB+
(seed_species_def[$seed_species_block::numSpecies]{
                                      $seedSpecies.add($seed_species_def.st); 
                                      $seed_species_block::numSpecies++;
                                      } LB+)* 
END (SEED)? SPECIES LB+;

seed_species_def[int counter]
scope{
  List molecules;
  BondList bonds;
}
@init{
  $seed_species_def::molecules = new ArrayList();
  $seed_species_def::bonds = new BondList();

}

:
  ({gParent.netGrammar}? INT | ) // We should deprecate this eventually --LAH
  ((STRING | INT) COLON)? 
  pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] {
 //There needs to need a space between species and the expression token, so we go back and make sure there was one
((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
} 
      WS 
     expression[gParent.memory] -> 
    seed_species_block(id={"S" + counter},concentration={$expression.text},name={$pre_species_def.text},molecules={$seed_species_def::molecules},
      firstBonds={$seed_species_def::bonds.getLeft()},secondBonds={$seed_species_def::bonds.getRight()});


pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment,Map<String,Register> lmemory]
scope{
  boolean constant;
  
}

:

//({gParent.netGrammar}? INT | )
 species_def2[molecules,bonds,"S" + speciesCounter]
 {
    $pre_species_def::constant = $species_def2.constant;
    $lmemory = $species_def2.lmemory;
 }

  
    
   
//DOT DOT DOT
;

//this call is used only for the seed species definitions section
species_def2[List molecules,BondList bonds,String speciesCounter] returns [Map<String,List<ReactionRegister>> listOfMolecules, String glabel, boolean constant,String compartment="",Map<String, Register> lmemory]
scope{
 int numMolecules; 
 String variableName;
 
}
@init{
  $species_def2::numMolecules =1;
  $listOfMolecules = new HashMap<String,List<ReactionRegister>>();
   $species_def2::variableName = "";
  $constant = false;
  $lmemory = new HashMap<String,Register>();
  //this is done so we can temporarily recognize whitespaces
((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
  gParent.paraphrases.push("in species def section");
}
@after{
((ChangeableChannelTokenStream)input).delChannel(HIDDEN);
//go back one space 
((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1);
((ChangeableChannelTokenStream)input).consume();

  gParent.paraphrases.pop();
}
:
   (AT scomp=STRING 
   
   {
    $compartment = $scomp.text;
   }
   COLON ({gParent.netGrammar}? COLON | ))?  
   
   (MOD slabel=STRING COLON COLON?
   {
    $lmemory.put($slabel.text,new Register(1.0,"observable"));
    $glabel = $slabel.text;
   }
   )? 
   (DOLLAR {$constant = true;})?
   
   (s1=species_element[bonds,speciesCounter + "_M" + $species_def2::numMolecules,$scomp.text] 
   {
    molecules.add(s1.st);
    $species_def2::numMolecules++;
    ReactionRegister.addElement($listOfMolecules,$s1.name,$s1.information);
    $species_def2::variableName += $s1.text;
    //in this step we are adding the label to local memory so that the expression grammar doesn't throw a variable not found error
    //i don't think it needs to make numerical sense.
    if($s1.myLabel != null){
      $lmemory.put($s1.myLabel,new Register(1.0,"observable"));
    }
   }
  (DOT s2=species_element[bonds,speciesCounter + "_M" + $species_def2::numMolecules,$scomp.text] 
        {molecules.add(s2.st);
        $species_def2::numMolecules++;
         $species_def2::variableName += "."+ $s2.text;
         if($s2.myLabel != null){
          $lmemory.put($s2.myLabel,new Register(1.0,"observable"));
         }
        })*
   )
   
    
   ->  molecules_def(id={$species_def2::variableName})
   
  
    
    
   ;
   
 
//this call is used only for reaction rules and observables
species_def[List molecules,BondList bonds,String speciesCounter] returns [Map<String,List<ReactionRegister>> listOfMolecules, String glabel, boolean constant,String compartment="",Map<String, Register> lmemory]
scope{
 int numMolecules; 
 String variableName;
}
@init{
  $species_def::numMolecules =1;
  $listOfMolecules = new HashMap<String,List<ReactionRegister>>();
  $species_def::variableName = "";
  $constant = false;
  $lmemory = new HashMap<String,Register>();
  //this is done so we can temporarily recognize whitespaces
((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
  gParent.paraphrases.push("in species def section");
}
@after{
((ChangeableChannelTokenStream)input).delChannel(HIDDEN);
//go back one space 
((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1);
((ChangeableChannelTokenStream)input).consume();

  gParent.paraphrases.pop();
}
:
   (AT scomp=STRING 
   
   {
    $compartment = $scomp.text;
   //((ChangeableChannelTokenStream)input).addChannel(HIDDEN);
   }
   COLON ({gParent.netGrammar}? COLON | ))?  
   
   (MOD slabel=STRING COLON COLON?
   {
    $lmemory.put($slabel.text,new Register(1.0,"observable"));
    $glabel = $slabel.text;
   }
   )? 
   
   (s1=species_element[bonds,speciesCounter + "_M" + $species_def::numMolecules,$scomp.text] 
   {
    molecules.add(s1.st);
    $species_def::numMolecules++;
    ReactionRegister.addElement($listOfMolecules,$s1.name,$s1.information);
    $species_def::variableName += $s1.text;
    //in this step we are adding the label to local memory so that the expression grammar doesn't throw a variable not found error
    //i don't think it needs to make numerical sense.
    if($s1.myLabel != null){
      $lmemory.put($s1.myLabel,new Register(1.0,"observable"));
    }
   }
  (DOT s2=species_element[bonds,speciesCounter + "_M" + $species_def::numMolecules,$scomp.text] 
        {molecules.add(s2.st);
        $species_def::numMolecules++;
         $species_def::variableName += "."+ $s2.text;
         if($s2.myLabel != null){
          $lmemory.put($s2.myLabel,new Register(1.0,"observable"));
         }
        })*
   )
   
    
   ->  molecules_def(id={$species_def::variableName})
   
  
    
    
   ;

species_element[BondList bonds,String upperID,String compartment] returns [String myLabel,String name, ReactionRegister information]
scope {
  List sites;
  String lcompartment;
  String lname;
}
@init{
getParentTemplate();
  $species_element::sites = new ArrayList();
  $species_element::lcompartment = compartment;
  $information = new ReactionRegister();
  $information.setNumBonds($bonds.getNumBonds());
  $information.setCompartment(compartment);
  gParent.paraphrases.push("in species element section");
}
@after{
  $information.setNumBonds($bonds.getNumBonds()-$information.getNumBonds());
 gParent.paraphrases.pop();
}

:
  s1= STRING {$name = $s1.text;$species_element::lname=$s1.text;} (label {$myLabel = $label.label;})? //label
  (LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN)? //If it's not a netfile it's necessary to add a '?' to allow for optional component syntax
 
  
  (ca=compartment_allocation 
  {
    $species_element::lcompartment = $ca.compartment; 
    $information.setCompartment($ca.compartment);
    $information.setBondList(bonds);
  })?
  -> list_molecule_def(id={upperID},name={$s1.text},sites={$species_element::sites},compartment = {$species_element::lcompartment},label={$myLabel});

compartment_allocation returns [String compartment]
:
  AT STRING
  {
    $compartment = $STRING.text;
  }
;
site_list[List sites,BondList bonds,String upperID]
scope{
   int numSites
}
@init{
  $site_list::numSites = 1;
  gParent.paraphrases.push("in species site list section");
}
@after{
gParent.paraphrases.pop();
}
:
  (s1=site_obs[bonds,upperID+"_C" + $site_list::numSites] {sites.add(s1.st);$site_list::numSites++;} 
  (COMMA s2=site_obs[bonds,upperID+"_C" + $site_list::numSites] {sites.add(s2.st);$site_list::numSites++;})*)? 
  ;
 
  
site_obs[BondList bonds,String upperID] returns [String myLabel]
scope{
  String defaultState;
  
  String bondIndicator;
}
@init{
  $site_obs::defaultState = null;
  $site_obs::bondIndicator = "0";
  }
:
  s1=STRING 
  (
  ((sos=site_obs_site[bonds,upperID]{$site_obs::bondIndicator = $sos.bond;})? site_obs_state?)
  | site_obs_state sos2=site_obs_site[bonds,upperID]{$site_obs::bondIndicator = $sos2.bond;}
  
     ) (label {$myLabel = $label.text;})?
  ->component_def(id={upperID},name={$s1.text},state={$site_obs::defaultState},numberOfBonds={$site_obs::bondIndicator});
  
site_obs_state: (TILDE state_label {$site_obs::defaultState = $state_label.text;});

site_obs_site[BondList bonds,String upperID] returns [String bond]
scope{
  int acc;
  String bondIndicator;
}
@init{
  $site_obs_site::acc = 0;
  $site_obs_site::bondIndicator = "";
  }
  :
        (EMARK ((PLUS {$site_obs_site::bondIndicator += $PLUS.text;})
        | (QMARK {$site_obs_site::bondIndicator += $QMARK.text;})
        | s1=bond_name 
                {
                $bonds.addElement($s1.text,upperID,$species_element::lname);  
                $site_obs_site::acc++;
                }))+
        {
           if($site_obs_site::acc >0)
            $bond = $site_obs_site::acc + $site_obs_site::bondIndicator;
           else if(!$site_obs_site::bondIndicator.equals(""))
            $bond = $site_obs_site::bondIndicator;
           else 
             $bond = "0";
        }
        ;
  
bond_name:
  INT | STRING ;
  
  
