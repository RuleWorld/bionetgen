import bionetgen

#mname="deleteMolecules"
mname="test_deleteMolecules"
model= bionetgen.bngmodel(mname+".bngl")
#with open(mname+"_pybng.bngl", "w") as f:
#    f.write(str(model)) # writes the changed model to new_model file
print(model.rules)