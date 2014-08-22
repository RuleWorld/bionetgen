{
'par_list':[
   {'name':"Nav",'value':"6.022e8",'unit':"",'type':"Avogadro number for 1 um^3"},
   {'name':"kp1",'value':"1.32845238e-7*1000*Nav",'unit':"/M.s",'type':"Bimolecular reaction"},
   {'name':"km1",'value':"0",'unit':"/s",'type':"Unimolecular reaction"},
   {'name':"kp2",'value':"2.5e-1*1000*Nav",'unit':"/M.s",'type':"Bimolecular reaction"},
   {'name':"km2",'value':"0",'unit':"/s",'type':"Unimolecular reaction"},
   {'name':"Lig_tot",'value':"6.0e3",'unit':"Number",'type':""},
   {'name':"Rec_tot",'value':"4.0e2",'unit':"Number",'type':""},
   {'name':"T",'value':"298.25",'unit':"K",'type':""},
   {'name':"Rs",'value':"0.002564",'unit':"um",'type':""},
   {'name':"gamma",'value':"0.5722",'unit':"",'type':"Euler's constant"},
   {'name':"KB",'value':"1.3806488e-19",'unit':"cm^2.kg/K.s^2",'type':"Boltzmann constant"}],

'mol_list':[
   {'name':"s1",'type':"3D",'dif':"KB*T/(6*PI*mu*Rs)"},
   {'name':"s2",'type':"3D",'dif':"KB*T/(6*PI*mu*Rs)"},
   {'name':"s3",'type':"3D",'dif':"KB*T/(6*PI*mu*(2)^(1/3)*Rs)"},
   {'name':"s4",'type':"3D",'dif':"KB*T/(6*PI*mu*(3)^(1/3)*Rs)"}],

'rxn_list':[
   {'reactants':"s2 + s1",'products':"s3",'fwd_rate':"2*kp1"},
   {'reactants':"s3",'products':"s2 + s1",'fwd_rate':"km1"},
   {'reactants':"s2 + s3",'products':"s4",'fwd_rate':"kp2"},
   {'reactants':"s4",'products':"s2 + s3",'fwd_rate':"2*km2"}],

'rel_list':[
   {'name':"Release_Site_s1",'molecule':"s1",'shape':"OBJECT",'orient':",",'quantity_type':"DENSITY",'quantity_expr':"Lig_tot/(Nav*1000)",'object_expr':"default_Sphere"},
   {'name':"Release_Site_s2",'molecule':"s2",'shape':"OBJECT",'orient':",",'quantity_type':"DENSITY",'quantity_expr':"Rec_tot/(Nav*1000)",'object_expr':"default_Sphere"}]
}