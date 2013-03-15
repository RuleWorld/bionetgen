parser grammar BNGGrammar_Parameters;

options {
  language = Java;
  output = template;
 
}
@members{
 public void getParentTemplate(){
 
  this.setTemplateLib(gParent.getTemplateLib());
 }
 
 private void addHidden(){
  ((ChangeableChannelTokenStream)input).addChannel(Token.DEFAULT_CHANNEL);
 }
 
 private void deleteHidden(){
 
 }
}

parameters_block [ Map<String,Register> memory, List parameters]
scope{
  Map<String,Register> lmemory;
}
@init{
  getParentTemplate();
  $parameters_block::lmemory = $memory;
 // System.out.println(gParent.netGrammar);
}

: BEGIN PARAMETERS LB+
(parameter_def[$memory] {$parameters.add($parameter_def.st);} LB+)*  

END PARAMETERS LB+;


parameter_def[Map<String,Register> lmemory]
:
({gParent.netGrammar}? INT |  ) // We should deprecate this eventually --LAH
        ((STRING | INT) COLON)?
        s1=STRING (BECOMES)? expression[lmemory]  {
        lmemory.put($s1.text,new Register($expression.value,"ConstantExpression"));
        }   -> parameter_def(id={$s1.text},value={$expression.text})
;