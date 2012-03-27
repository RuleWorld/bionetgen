parser grammar BNGGrammar_MoleculeDef;

options {
  language = Java;
  output = template;
 
}
@members{
 public void getParentTemplate(){
 
  this.setTemplateLib(gParent.getTemplateLib());
 }
}

molecule_types_block
@init{
getParentTemplate();
 gParent.paraphrases.push("in molecule types section");
}
@after{
  gParent.paraphrases.pop();
}
:
 BEGIN MOLECULE TYPES (molecule_def 
 
 {
 gParent.molecules.add($molecule_def.st);
 //gParent.variableName.add($molecule_def.text);
 }
 
 )* END MOLECULE TYPES;

molecule_def
scope{
  List sites;
}
@init{
  $molecule_def::sites = new ArrayList();
}
: ({gParent.netGrammar}? INT | ) STRING (LPAREN (
  (s1=site_def {$molecule_def::sites.add($s1.st);}
  (COMMA s2=site_def {$molecule_def::sites.add($s2.st);})*) 
  | //nothing 
  ) RPAREN) //? adding optional parenthesis require more work
 
  
  -> molecules_def(id={$STRING.text},sites={$molecule_def::sites});


site_def
scope{
  List<String> states;
}
@init{
  $site_def::states = new ArrayList<String>();
}
: STRING (TILDE state_label {$site_def::states.add($state_label.text);})* -> sites_def(id={$STRING.text},states={$site_def::states});

state_label: (STRING | INT | QMARK)+;

