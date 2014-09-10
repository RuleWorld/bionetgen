par_list={
   1:{'name':"Nav",'value':"6.022e8",'unit':"",'type':"Avogadro number for 1 um^3"},
   2:{'name':"rxn_layer_t",'value':"0.01",'unit':"um",'type':""},
   3:{'name':"vol_EC",'value':"20.0",'unit':"um^3",'type':""},
   4:{'name':"sa_PM",'value':"0.4/rxn_layer_t",'unit':"um^2",'type':""},
   5:{'name':"vol_CP",'value':"4.0",'unit':"um^3",'type':""},
   6:{'name':"sa_NM",'value':"0.1/rxn_layer_t",'unit':"um^2",'type':""},
   7:{'name':"vol_NU",'value':"1.0",'unit':"um^3",'type':""},
   8:{'name':"sa_EM",'value':"0.05/rxn_layer_t",'unit':"um^2",'type':""},
   9:{'name':"vol_EN",'value':"0.5",'unit':"um^3",'type':""},
   10:{'name':"kp_LR",'value':"0.1*Nav",'unit':"/M.s",'type':"Bimolecular reaction"},
   11:{'name':"km_LR",'value':"1.0",'unit':"/s",'type':"Unimolecular reaction"},
   12:{'name':"kp_LL",'value':"0.1*Nav",'unit':"/M.s",'type':"Bimolecular reaction"},
   13:{'name':"km_LL",'value':"1.0",'unit':"/s",'type':"Unimolecular reaction"},
   14:{'name':"L0",'value':"1000",'unit':"Number",'type':""},
   15:{'name':"R0",'value':"200",'unit':"Number",'type':""},
   16:{'name':"T",'value':"298.25",'unit':"K",'type':""},
   17:{'name':"h",'value':"rxn_layer_t",'unit':"um",'type':""},
   18:{'name':"Rs",'value':"0.002564",'unit':"um",'type':""},
   19:{'name':"Rc",'value':"0.0015",'unit':"um",'type':""},
   20:{'name':"gamma",'value':"0.5722",'unit':"",'type':"Euler's constant"},
   21:{'name':"KB",'value':"1.3806488e-19",'unit':"cm^2.kg/K.s^2",'type':"Boltzmann constant"},
   22:{'name':"mu_EC",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"},
   23:{'name':"mu_PM",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"},
   24:{'name':"mu_CP",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"},
   25:{'name':"mu_NM",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"},
   26:{'name':"mu_NU",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"},
   27:{'name':"mu_EM",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"},
   28:{'name':"mu_EN",'value':"1e-9",'unit':"kg/um.s",'type':"viscosity"}}

mol_list={
   1:{'name':"s1",'type':"3D",'dif':"KB*T/(6*PI*mu_EC*Rs)"},
   2:{'name':"s2",'type':"2D",'dif':"KB*T*LOG((mu_PM*h/(Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)"},
   3:{'name':"s3",'type':"3D",'dif':"KB*T/(6*PI*mu_CP*Rs)"},
   4:{'name':"s4",'type':"2D",'dif':"KB*T*LOG((mu_PM*h/(Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)"},
   5:{'name':"s5",'type':"3D",'dif':"KB*T/(6*PI*mu_EC*(2)^(1/3)*Rs)"},
   6:{'name':"s6",'type':"2D",'dif':"KB*T*LOG((mu_PM*h/(Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)"},
   7:{'name':"s7",'type':"2D",'dif':"KB*T*LOG((mu_PM*h/(SQRT(2)*Rc*(mu_EC+mu_CP)/2))-gamma)/(4*PI*mu_PM*h)"}}

rxn_list={
   1:{'reactants':"s1' + s2'",'products':"s4'",'fwd_rate':"kp_LR"},
   2:{'reactants':"s1 + s1",'products':"s5",'fwd_rate':"0.5*kp_LL"},
   3:{'reactants':"s5' + s2'",'products':"s6'",'fwd_rate':"2*kp_LR"},
   4:{'reactants':"s4'",'products':"s1' + s2'",'fwd_rate':"km_LR"},
   5:{'reactants':"s1' + s4'",'products':"s6'",'fwd_rate':"kp_LL"},
   6:{'reactants':"s4' + s4'",'products':"s7'",'fwd_rate':"0.5*kp_LL"},
   7:{'reactants':"s5",'products':"s1 + s1",'fwd_rate':"km_LL"},
   8:{'reactants':"s6' + s2'",'products':"s7'",'fwd_rate':"kp_LR"},
   9:{'reactants':"s6'",'products':"s5' + s2'",'fwd_rate':"km_LR"},
   10:{'reactants':"s7'",'products':"s6' + s2'",'fwd_rate':"2*km_LR"},
   11:{'reactants':"s6'",'products':"s4' + s1'",'fwd_rate':"km_LL"},
   12:{'reactants':"s7'",'products':"s4' + s4'",'fwd_rate':"km_LL"}}

rel_list={
   1:{'name':"Release_Site_s1",'molecule':"s1",'shape':"OBJECT",'orient':",",'quantity_type':"DENSITY",'quantity_expr':"L0/(Nav*vol_EC)",'object_expr':" - CP[ALL]"},
   2:{'name':"Release_Site_s2",'molecule':"s2",'shape':"OBJECT",'orient':"\'",'quantity_type':"DENSITY",'quantity_expr':"R0/vol_PM",'object_expr':"CP[PM]"}}