#!/usr/bin/env python

def bnglReaction(reactant,product,rate,translator=[]):
	finalString = ''
	for index in range(0,len(reactant)):
		finalString += printTranslate(reactant[index],translator)
		if index < len(reactant) -1:
			finalString += ' + '
	finalString += ' -> '
	for index in range(0,len(product)):
		finalString += printTranslate(product[index],translator) 
		if index < len(product) -1:
			finalString += ' + '
	finalString += ' ' + rate
	return finalString
	

def printTranslate(chemical,translator=[]):
    if chemical not in translator:
        return chemical + '()'
    else:
        return printSpecies(translator[chemical][0],translator[chemical][1])


def printSpecies(label,definition):
    molecules = []
    for tag, species in zip(label,definition):
        components = []
        for member in species:
            if len(member) == 2:
                components.append('{0}!{1}'.format(member[0], member[1]))
            else:
                components.append('{0}'.format(member[0]))
        molecules.append('{0}({1})'.format(tag,",".join(components)))
    return ".".join(molecules)
    
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

