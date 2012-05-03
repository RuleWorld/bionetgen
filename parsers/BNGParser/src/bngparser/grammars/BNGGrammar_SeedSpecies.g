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
   gParent.paraphrases.push("in seed species section");
   getParentTemplate();
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
(STRING COLON)? pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] {
 //There needs to need a space between species and the expression token, so we go back and make sure there was one
((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
} 
      WS 
     expression[gParent.memory] -> 
    seed_species_block(id={"S" + counter},concentration={$expression.text},name={$pre_species_def.text},molecules={$seed_species_def::molecules},
      firstBonds={$seed_species_def::bonds.getLeft()},secondBonds={$seed_species_def::bonds.getRight()});


pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment]
scope{
  boolean constant;
  
}

:

({gParent.netGrammar}? INT | )
 species_def[molecules,bonds,"S" + speciesCounter]
 {
    $pre_species_def::constant = $species_def.constant;
 }

  
    
   
//DOT DOT DOT
;


species_def[List molecules,BondList bonds,String speciesCounter] returns [Map<String,List<ReactionRegister>> listOfMolecules, boolean constant, String compartment=""]
scope{
 int numMolecules; 
 String variableName;


}
@init{
  $species_def::numMolecules =1;
  $listOfMolecules = new HashMap<String,List<ReactionRegister>>();
   //We cannot have spaces between the elements of a species definition, so we activate space recognition. Should there be a space
   //the parser will not continue with this rule
 //  ((ChangeableChannelTokenStream)input).setChannel(Token.DEFAULT_CHANNEL|HIDDEN);
  $species_def::variableName = "";
  $constant = false;
 
}
:
   (AT scomp=STRING 
   
   {
    $compartment = $scomp.text;
   }
   COLON ({gParent.netGrammar}? COLON | ))?  (DOLLAR {$constant = true;})?
   
   (s1=species_element[bonds,speciesCounter + "_M" + $species_def::numMolecules,$scomp.text] 
   {
    molecules.add(s1.st);
    $species_def::numMolecules++;
    ReactionRegister.addElement($listOfMolecules,$s1.name,$s1.information);
    $species_def::variableName += $s1.text;
   }
  (DOT s2=species_element[bonds,speciesCounter + "_M" + $species_def::numMolecules,$scomp.text] 
        {molecules.add(s2.st);
        $species_def::numMolecules++;
         $species_def::variableName += "."+ $s2.text;
        })*
   {
   //We are done with space recognition so we turn it off and get the next token that is not a space
  // ((ChangeableChannelTokenStream)input).setChannel(Token.DEFAULT_CHANNEL);
  // ((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).skipOffTokenChannels(((ChangeableChannelTokenStream)input).index()));
 
   }
   )
   
    {
    //if(!gParent.variableName.contains($species_def::variableName) && gParent.conserveNewMolecules){
      //gParent.variableName.add($species_def::variableName);
     // gParent.molecules.add(new StringTemplate($species_def::variableName));
     // }
    }
    
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
  
}
@after{
  $information.setNumBonds($bonds.getNumBonds()-$information.getNumBonds());
}

:
  s1= STRING {$name = $s1.text;$species_element::lname=$s1.text;} (label {$myLabel = $label.text;})? (LPAREN site_list[$species_element::sites,bonds,upperID] RPAREN)?
  (AT s2=STRING 
  {
    $species_element::lcompartment = $s2.text; 
    $information.setCompartment($s2.text);
    $information.setBondList(bonds);
  })? 
  -> list_molecule_def(id={upperID},name={$s1.text},sites={$species_element::sites},compartment = {$species_element::lcompartment});
  
site_list[List sites,BondList bonds,String upperID]
scope{
   int numSites
}
@init{
  $site_list::numSites = 1;
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
  
  
