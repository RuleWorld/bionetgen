parser grammar NETGrammar;

options {
  language = Java;
  output = template;
  tokenVocab = BNGLexer;
  
}
import BNGGrammar_Expression,BNGGrammar_Parameters,BNGGrammar_SeedSpecies,BNGGrammar_MoleculeDef,BNGGrammar_Observables,BNGGrammar_ReactionRules;
@header{
  package bngparser.grammars;
  import bngparser.grammars.BNGLexer;
  import org.antlr.stringtemplate.*;
  import bngparser.dataType.BondList;
  import java.util.Map;
  import bngparser.methods.ReactionAction;
  import bngparser.dataType.Register;
  import bngparser.methods.InvertBidirectional;
  import bngparser.dataType.ChangeableChannelTokenStream;
  import bngparser.dataType.ReactionRegister;
  import bngparser.dataType.Compartments;
  import bngparser.methods.GenericMethods;
}
@members{
  public Stack<String> paraphrases = new Stack<String>();
  public Map<String,Register> memory = new HashMap<String,Register>();
  
  private Map<String, Map<String,String>> options = new HashMap<String, Map<String,String>>();

  public boolean netGrammar = true;
  
  //This is a pretty complex flag. 
  //Surface compartments require pretty special treatment as opposed to volume compartments
  //If this flag is not activated, it will treat all compartments the same, however if it is activated
  // it will generate the corresponding surface syntax. This is still experimental
  public boolean compartmentToSurface = true;
  public List<StringTemplate> molecules = new ArrayList<StringTemplate>();
  public List<StringTemplate> reactions = new ArrayList<StringTemplate>();
  public List<StringTemplate> groups = new ArrayList<StringTemplate>();
  public Map<String,String> speciesMap = new HashMap<String,String>();
  
  //this object contains a list of all comparments and their characteristics.
  public Compartments compartmentList = new Compartments();
  //this map contains a list of the surface compartments and which species (stringtemplates)are associated with them
  public Map<String, List<StringTemplate>> compartmentSurfaces = new HashMap<String,List<StringTemplate>>();
  //the same as the previous one but in this one we keep track of all compartments and only 
  //molecule names
  public Map<String, List<String>> compartmentMolecules = new HashMap<String,List<String>>();
  public List<String> surfaces = new ArrayList<String>();
  public String cyto_volume = "3.0e-12";
  public String ecm_volume = "1.0e-10";
  public double cyto_volume_value = 3.0e12;
  public double ecm_volume_value = 1.0e-10;
  public double avogadro_constant = 6.02e23;
  @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    String msg = super.getErrorMessage(e,tokenNames);
    if(paraphrases.size(  )>0) {
      String paraphrase = paraphrases.peek();
      msg += " " + paraphrase;
    }
    return msg;
  }
  
  public void addOption(String element, String option, String setting){
    if(!options.containsKey(element)){
      options.put(element,new HashMap<String,String>());
    }
    
    (options.get(element)).put(option,setting);
  }
  
  public void addOptions(String element, Map<String,String> option){
    if(!options.containsKey(element)){
      options.put(element,option);
    }
    else{
      options.get(element).putAll(option);
    }
  }
  
  public void addSurfaceToCompartment(String surfaceName,StringTemplate template ){
     //String parent = compartmentList.getParentCompartment(surfaceName);
     if(!compartmentSurfaces.containsKey(surfaceName)){
        compartmentSurfaces.put(surfaceName,new ArrayList<StringTemplate>());
     }
     compartmentSurfaces.get(surfaceName).add(template);
  }
  
    public void addMoleculeToCompartment(String compartment,String name ){
     //String parent = compartmentList.getParentCompartment(surfaceName);
     if(!compartmentMolecules.containsKey(compartment)){
        compartmentMolecules.put(compartment,new ArrayList<String>());
     }
     compartmentMolecules.get(compartment).add(name);
  }
  
   public String getCompartment(String molecule){
    String temp = "";
    for(String compartment: compartmentMolecules.keySet()){
      if(compartmentMolecules.get(compartment).contains(molecule))
        return compartment;
    }
    
    return temp;
    
  }
}

/**
Rewriting the seed species blook for netfiles. This is a modified version that includes some helper methods
for the generation of the mdl structure
**/
seed_species_block[List seedSpecies]
scope{
 int numSpecies;
 }
@init{
 $seed_species_block::numSpecies = 1;
   paraphrases.push("in seed species section");

}
@after{
  paraphrases.pop();
}
: BEGIN (SEED)? SPECIES LB+
(seed_species_def[$seed_species_block::numSpecies]{
                                      //System.out.println("aaaaa " + $seed_species_def.compartment);
                                      if(!$seed_species_def.concentration.equals("0")){
	                                      if(!compartmentToSurface || compartmentList.isVolume($seed_species_def.compartment))
	                                        $seedSpecies.add($seed_species_def.st);
	                                      else
	                                        addSurfaceToCompartment($seed_species_def.compartment,$seed_species_def.st);
                                      }
                                      addMoleculeToCompartment($seed_species_def.compartment,"S" + $seed_species_block::numSpecies);
                                      $seed_species_block::numSpecies++;
                                      //}
                                      } LB+)* 
END (SEED)? SPECIES LB+
;


/**
This method reads a line in the seed species section and creates the corresponding generator in the mdl file

**/
seed_species_def[int counter] returns [String compartment, String concentration]
scope{
  List molecules;
  BondList bonds;
  String location;
  boolean isVolume;
}
@init{
  $seed_species_def::molecules = new ArrayList();
  $seed_species_def::bonds = new BondList();
}

:
 pre=pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] {
 //There needs to need a space between species and the expression token, so we go back and make sure there was one
//((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
$compartment = $pre.compartment;
$seed_species_def::isVolume = !compartmentToSurface || compartmentList.isVolume(pre.compartment);
 //System.out.println("aaaa " +input);
 //((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
} 
       
     expression[memory] 
     {
      $concentration = $expression.text;
      //fixme: using static coordinates for now. Fix for something better
      if(compartmentToSurface &&  compartmentList.isOuterCompartment($compartment)){
        $seed_species_def::location = "-0.15,-0.15,-0.15";
      }
      else{
        $seed_species_def::location = "0,0,0";
      }
     }
     -> 
    seed_species_block(id={counter},concentration={$expression.text},name={$pre_species_def.text},molecules={$seed_species_def::molecules},
      firstBonds={$seed_species_def::bonds.getLeft()},secondBonds={$seed_species_def::bonds.getRight()},
      isVolume={$seed_species_def::isVolume},location={$seed_species_def::location})
      ;
     


pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment]
scope{
  boolean constant;
  
}

:
INT  

 species_def[molecules,bonds,"S" + speciesCounter]
 {
    $pre_species_def::constant = $species_def.constant;
     speciesMap.put($species_def.text,$INT.text);
     $compartment = $species_def.compartment;
 }
 ;
  
 
/**
Syntax definition for the net file only groups section
**/
groups_block:
BEGIN GROUPS LB+
  (group_line LB+)*
END GROUPS LB+

;


group_line:
INT STRING group_list[$STRING.text] {groups.add($group_list.st);}
;

group_list[String name]
scope{
  int counter;
  List<StringTemplate> groupList;
}
@init{
  $group_list::counter = 0;
  $group_list::groupList = new ArrayList<StringTemplate>();
}:
  g1=group_element[$name,$group_list::counter] {$group_list::groupList.add($g1.st);$group_list::counter++;}
  (COMMA g2=group_element[$name,$group_list::counter] {$group_list::groupList.add($g2.st);$group_list::counter++;})*
  -> groups_groups(groupList={$group_list::groupList},name={$name})
  ;
  
group_element[String name,int counter]:
  i1=INT -> groups_block(id = {$i1.text}) 
  | (i2=INT TIMES i3=INT) -> groups_block(id = {$i3.text},times = {$i2.text});
  

/**
entry point
**/
prog
scope {
  List parameters;
  //List molecules;
  List seedSpecies;
  List observables;
  List reactionRules;
  List functions;
  List compartments;
 // Map<String,Register> memory;
  Stack elements;
  Map<String,List<StringTemplate>> surfaces;
  List<StringTemplate> moleculeDeclarations;
}
@init {
  $prog::parameters = new ArrayList();
 
  $prog::seedSpecies = new ArrayList();
  $prog::reactionRules = new ArrayList();
  $prog::observables = new ArrayList();
  $prog::functions = new ArrayList();
  $prog::compartments = new ArrayList();
 // memory = new HashMap<String,Register>();
  $prog::elements = new Stack();
  $prog::surfaces = new HashMap<String,List<StringTemplate>>();
  $prog::moleculeDeclarations = new ArrayList<StringTemplate>();
  paraphrases.push("in model");
}
@after{
  paraphrases.pop();
}
:
LB*
SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI LB+
(program_block)* 

{

//generating the code for surfaces and volumes
  for(String element: compartmentSurfaces.keySet()){
    if(!compartmentList.isSurface(element))
      continue;
    StringTemplate temp;
    //temp = template("create_surface_region");
    
    //this is the equivalent of manually calling a template from a stg file
    //where the options aray contains the parameters, and the templateLib calls the method
    //This is necessary because of the stark differences in mdl syntax between a surface and a volume
    STAttrMap options = new STAttrMap();
    options.put("name", element);
    options.put("molecules",compartmentSurfaces.get(element));
    temp = templateLib.getInstanceOf("create_surface_region",options);
    if(!$prog::surfaces.containsKey(compartmentList.getParentCompartment(element))){
      $prog::surfaces.put(compartmentList.getParentCompartment(element),new ArrayList<StringTemplate>());
    }
    $prog::surfaces.get(compartmentList.getParentCompartment(element)).add(temp);
   // System.out.println(temp);
  }
  for(String tag: speciesMap.keySet()){
    String compartment = getCompartment("S" + speciesMap.get(tag));
    String dimension = "3";
    if(compartmentToSurface)
      dimension = compartmentList.getDimension(compartment);;
    String difussion;
    STAttrMap options = new STAttrMap();
    options.put("index",speciesMap.get(tag));
    options.put("name",tag);
    options.put("dimension",dimension);
    if(dimension.equals("2") && compartmentToSurface){
      difussion = "1e-6";
    }
    else{
      difussion="1e-6";
    }
    options.put("difussion",difussion);
    StringTemplate temp = templateLib.getInstanceOf("define_molecules",options);
    $prog::moleculeDeclarations.add(temp);
    //System.out.println("hello " + compartment + compartmentList.getDimension(compartment));
  }
}

EOF 

 -> prog2(parameters={$prog::parameters},molecules={$prog::moleculeDeclarations},species={$prog::seedSpecies},reactions={reactions},
                            observables={groups},functions={$prog::functions}, compartments={$prog::compartments},
                            boxes={$prog::surfaces},compartmentsSurface={compartmentToSurface});

version_def: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI;

//program_body:  ;



program_block
: parameters_block[memory,$prog::parameters]
| molecule_types_block
| seed_species_block[$prog::seedSpecies]
| observables_block[$prog::observables]
| reaction_rules_block[$prog::reactionRules]
| functions_block
| compartments_block
| reactions_block[reactions]
| groups_block
//| set_option
;


functions_block
@init{
  paraphrases.push("in function block");
}
@after{
  paraphrases.pop();
}
:
      BEGIN FUNCTIONS LB+
       (function_def LB+{$prog::functions.add($function_def.st);})*     
      END FUNCTIONS LB+
;

function_def:
    s1=STRING LPAREN (STRING)? RPAREN (BECOMES)?  expression[memory] ->  functions_block(id={$s1.text},
                            referencesName={$expression.reference.keySet()},referencesType={Register.getTypes($expression.reference)},expression={$expression.text})
;
//http://bionetgen.org/index.php/Compartments_in_BNGL
compartments_block:

  BEGIN COMPARTMENTS LB+
      (compartment LB+{$prog::compartments.add($compartment.st);})*
  END COMPARTMENTS LB+;
  
compartment:
   s1=STRING INT s3=expression[memory] 
   {
      compartmentList.addCompartment($s1.text,$INT.text,($s3.value).toString());
   }
   (s2=STRING
   {
      compartmentList.addContainer($s1.text,$s2.text);
   }
   )? 
   -> compartments_block(id={$s1.text},dimensions={$INT.text},size={$s3.value},outside={$s2.text}) 
;
  
label returns [String label]:
  MOD STRING{$label = $STRING.text;};
  
dereference returns [String label]:
  AMPERSAND STRING{$label = $STRING.text;};  
  
  

 reactions_block[List<StringTemplate> templates]:
BEGIN REACTIONS LB+
  (INT reaction LB+
  {
    $templates.add($reaction.st);
  })*
END REACTIONS LB+
;

reaction
scope{
  List<String> reactant;
  List<String> product;
  double volume;
}
@init{
  $reaction::reactant = new ArrayList<String>();
  $reaction::product = new ArrayList<String>();
  $reaction::volume = 0.0;
}
:
 r1=reactant[$reaction::reactant] r2=reactant[$reaction::product] expression[memory]
 {
  if($r1.size>1){
	  if(!compartmentToSurface){
	    $reaction::volume =new Double(compartmentList.getECMVolume()) *avogadro_constant;
	    //$reaction::volume = ecm_volume_value *avogadro_constant;
	  } 
	  else if($r1.volume.equals(ecm_volume) || $r2.volume.equals(ecm_volume)){
	  //System.out.println(compartmentList.getECMVolume());
	  $reaction::volume = new Double(compartmentList.getECMVolume()) *avogadro_constant/1e4;
	  //  $reaction::volume = ecm_volume_value *avogadro_constant;
	  }
	  else if($r1.volume.equals(cyto_volume) || $r2.volume.equals(cyto_volume)){
	    $reaction::volume =new Double(compartmentList.getCytoplasmicVolume()) *avogadro_constant/1e4;
	    //$reaction::volume = cyto_volume_value *avogadro_constant;
	  }
	  else{
	  //$reaction::volume = 1*0.01/new Double(compartmentList.getMembraneVolume())*1e15;
	  
	  //fixme: this should be automatically calculated
	    $reaction::volume = 4*3.141592*5*5;
	  }
  }
  else{
    $reaction::volume = 1;
  }
 
 }
 -> reactions(reactant={$reaction::reactant},product={$reaction::product}, rate={$expression.value*$reaction::volume},
  surfaceCompartment={$r1.surfaceCompartment||$r2.surfaceCompartment},reactantOrientation = {$r1.orientation},productOrientation={$r2.orientation},
  factor={""})
;


/**
reactant section
**/
reactant[List<String> elements] returns [boolean surfaceCompartment=false, String volume="1", List<String> orientation, int size=0]
@init{
  $orientation = new ArrayList<String>();
}
:

  i1=INT
  {
    $elements.add($i1.text);
    $size++;
  }
   
  (COMMA i2=INT
  {
    $elements.add($i2.text);
    $size++;
  }
  )*
  {
    if(compartmentToSurface){
	    for(String molecules: $elements){
	      if(compartmentList.isSurface(getCompartment("S" + molecules))){
	        $surfaceCompartment =  true;
	      }
	      
	      //here we are assiginging a preset volume to a compartment depending if its the cytoplasm or the ecm
	      else if(compartmentList.isOuterCompartment(getCompartment("S" + molecules))){
	        $volume = ecm_volume;
	      }
	      else{
	        $volume = cyto_volume;
	      }
	    }
    }
    
    

    if(compartmentToSurface){
	    for(String molecules: $elements){
	        if(compartmentList.isOuterCompartment(getCompartment("S" + molecules)) || compartmentList.isSurface(getCompartment("S" + molecules))){
	          $orientation.add("'");
	        }
	        else{
	          $orientation.add(",");
	        }
	    }
    }
    
  
  }
;

