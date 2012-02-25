#!/usr/bin/env python

def bnglReaction(reactant,product,rate):
	finalString = ''
	for index in range(0,len(reactant)):
		finalString += reactant[index] + '()'
		if index < len(reactant) -1:
			finalString += ' + '
	finalString += ' -> '
	for index in range(0,len(product)):
		finalString += product[index] + '()'
		if index < len(product) -1:
			finalString += ' + '
	finalString += ' ' + rate
	return finalString
	

def bnglFunction(rule,functionTitle):
	
	finalString = '%s = %s' % (functionTitle,rule)
	return finalString

	
def finalText(param,molecules,species,observables,rules,functions,fileName):
	output = open(fileName,'w')
	output.write('begin model\n')
	output.write(sectionTemplate('parameters',param))
	output.write(sectionTemplate('molecules',molecules))
	output.write(sectionTemplate('seed species',species))
	output.write(sectionTemplate('functions',functions))
	output.write(sectionTemplate('observables',observables))
	output.write(sectionTemplate('rules',rules))
	output.write('end model\n')
	output.write('generate_network();\n')
	output.write('simulate_ode({t_end=>400,n_steps=>50});')
	
def sectionTemplate(name,content):
	section = 'begin %s\n' % name
	temp = ['\t%s\n' % line for line in content]
	section += ''.join(temp)
	section += 'end %s\n' % name
	return section

