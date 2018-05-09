import sys
import math
import random
from matplotlib import pyplot as plt
import numpy
import copy
import time
#NOTES: In ptempest, the temperature and step sizes never adapt on the same iteration. If temperature is adapting, step size does not. Is this necessary? It seems arbitrary
#Chain and PT classes are instantiated with full random number streams for repeatability. This is a good idea, right? Or is it better to instantiate them with seeds and generate the streams inside?
class state_vector: #multivariate Markov chain
	def __init__(self,x,prior,p1,p2,simulate_function,likelihood_model,data): #instantitate it with the position, prior definition, prior parameters
		self.x = x
		self.prior = prior;
		self.p1 = p1;
		self.p2 = p2;
		self.num_par = len(x)
		self.simulate_function = simulate_function
		self.likelihood_model = likelihood_model
		self.data = data
	def priorpdf(self):
		priorpdf_array = [0 for i in range(0,self.num_par)]
		for i in range(0,self.num_par): #Iterate over each parameter 
			parameter_pos = self.x[i]
			parameter_prior = self.prior[i]
			parameter_p1 = self.p1[i]
			parameter_p2 = self.p2[i]
			if parameter_prior=="uniform":
				priorpdf_array[i] =  0 if (parameter_pos<parameter_p2 and parameter_pos>parameter_p1) else -float("inf")
			elif parameter_prior=="normal":
				priorpdf_array[i] = -((parameter_pos-parameter_p1)**2)/(2*(parameter_p2**2))
			elif parameter_prior=="laplace":
				priorpdf_array[i] = -math.fabs(parameter_pos-parameter_p1)/parameter_p2
			elif parameter_prior=="exp":
				if parameter_pos<0:
					priorpdf_array[i] = -float("inf");
				else:
					priorpdf_array[i] = -(parameter_pos)/parameter_p1
		return priorpdf_array
	def energy(self):
		return numpy.sum(-numpy.array(self.priorpdf()))-self.likelihood_model(self.x,self.simulate_function,self.data)
class chain:
	def __init__(self,init,propose,beta,relstep,r_walk,r_accept,*argv):
		self.current = init; #initial state
		self.propose = propose; #proposal function
		self.states = []#[init] #state chain	
		self.beta = beta; #temperature parameter
		self.iteration = 0;
		self.energy_curr = self.current.energy()
		self.relstep = [float(relstep/beta) for i in range(0,len(init.x))] #the step sizes are scaled in accordance with the temperature. *This is how it is done in ptempest*.
		self.r_walk = r_walk
		self.r_accept = r_accept
		self.step_acceptance = []
		self.relstep_history = []
		self.adapt_relstep = 0 #Do we want adaptive stepping 0/1
		self.energy_chain = []#[self.energy_curr]
		for arg in argv:
			for key in arg.keys():
				if key=="chain_config_options":
					vals = arg[key]
					self.adapt_last = vals['adapt_last']
					self.adapt_relstep_interval = vals['adapt_relstep_interval']
					self.optimal_step_acceptance = vals['optimal_step_acceptance']
					self.min_adaptation_factor = vals['min_adaptation_factor']
					self.max_adaptation_factor = vals['max_adaptation_factor']
					self.adapt_relstep_rate = vals['adapt_relstep_rate']
					self.adapt_relstep = 1
	def step(self,flag):
		self.states.append(self.current)
		self.energy_chain.append(self.energy_curr)		
		tt = time.clock()
		self.relstep_history.append(self.relstep)
		new = self.propose(self.current,self.iteration,self.relstep,self.r_walk)
		print(self.r_walk[self.iteration])
		logpn,logcn = new.energy(),self.energy_curr
		r = self.r_accept[self.iteration]
		dE = logpn-logcn
		try:
			h = min(1,math.exp(-self.beta*dE))
		except OverflowError:
			h = float('inf')
		if flag==1:
			print("current parameter: ",self.current.x," current energy ",logcn,"\n")
			print("proposed parameter: ",new.x," proposed energy ",logpn,"\n")
			print("Energy difference (dE = logpn-logcn): ",logpn-logcn,"\n")
			try:
				print("Random number: ",r," exp(-dE)",math.exp(-dE),"\n")
			except OverflowError:
				print("Random number: ",r," exp(-dE)",float('inf'),"\n")
			print("Acceptance probability (h) = min(1,exp(-dE)): ",h,"\n")
		if r<h:
			if flag==1:
				print("Accepted","\n\n")
			self.current = new
			self.energy_curr = logpn
			self.step_acceptance.append(1)
			self.steps_accepted_current_run = self.steps_accepted_current_run + 1
		else:
			if flag==1:
				print("Rejected","\n\n")
			self.step_acceptance.append(0)
		#self.states.append(self.current)
		#self.energy_chain.append(self.energy_curr)
		#print "Step time: "+str(time.clock()-tt)
	def adapt_step_size(self,flag):
		step_acceptance_rate = float(sum(self.step_acceptance[self.iteration+1-self.adapt_relstep_interval:self.iteration+1]))/float(self.adapt_relstep_interval)
		old_relstep = copy.copy(self.relstep)
		adaptation_factor = (float(step_acceptance_rate/self.optimal_step_acceptance))**self.adapt_relstep_rate
		adaptation_factor = min(max(self.min_adaptation_factor,adaptation_factor),self.max_adaptation_factor)
		self.relstep = [x*adaptation_factor for x in old_relstep]
		if flag==1:
			print("\n Adapting step size, old step size: " + str(old_relstep) +" new step size: "+str(self.relstep)+"\n")
	def run(self,n,*argv):
		flag = 0
		self.steps_accepted_current_run = 0
		for arg in argv:
			if arg=="verbose":
				flag = 1
		n = n+self.iteration
		for i in xrange(self.iteration,n):
			if flag==1:
				print("step number ",i,"--------------------------------\n")
			self.step(flag)
			if self.adapt_relstep ==1 and (self.iteration+1)%self.adapt_relstep_interval==0 and self.iteration<self.adapt_last:
				self.adapt_step_size(flag)
			self.iteration = i+1 #last completed step (will help with chain continuations)
	def save_chain(self,basename="chain_",foldername=".",*args): #Default folder to save is the current folder.
		#If nothing is specified, save all the attributes of this chain as individual text files. Else just save the attributes listed in args(TODO).
		#Save lists or lists of lists in separate files named with the attribute name; Save scalars all together in one file; Don't save functions or instances.
		attr = vars(self)
		f = open('miscellaneous_chain_scalars.txt','w')
		for key in attr.keys():
			val = attr[key]
			if isinstance(val,list):
				#Check list element type
				if all(isinstance(x, int) or isinstance(x, float) for x in val): 
					f1 = open(basename+key+'.txt','w+')
					f1.writelines([str(x)+"\n" for x in val])
					f1.close()
				elif all(isinstance(x, list) for x in val):
					list_el = val[0]
					if all(isinstance(x, int) or isinstance(x, float) for x in list_el): 
						f1 = open(basename+key+'.txt','w+')
						for y in val:
							f1.writelines([str(x)+"\t" for x in y])
							f1.writelines("\n")
						f1.close()
				elif key=="states":
					states = [el.x for el in val]
					f1 = open(basename+key+'.txt','w+')
					for y in states:
						f1.writelines([str(x)+"\t" for x in y])
						f1.writelines("\n")
					f1.close()
			elif isinstance(val,int) or isinstance(val,float):	
				f.write(key+"\t"+str(val)+"\n")
		f.close()			
class parallel_tempering:
	def __init__(self,c,nswap,nstep,r_swap,*argv): #c is a list of chain objects from lowest temperature to highest temperature
		self.c = c
		self.nswap = nswap
		self.nstep = nstep
		self.param_curr = [i.current for i in c]
		self.energy_curr =[i.energy_curr for i in c]
		self.param_chain = [self.param_curr]
		self.energy_chain = [self.energy_curr]
		self.nchains = len(self.c)
		self.swap_history = [[[] for i in range(0,self.nchains-1)] for i in range(0,self.nswap)]
		self.swap_counter = 0
		self.r_swap = r_swap
		self.swap_time = []
		for arg in argv:
			for key in arg.keys():
				if key=="pt_config_options":
					vals = arg[key]
					#Temperature adaptation options
					self.beta_adapt_last = vals['beta_adapt_last']
					self.adapt_beta_interval = vals['adapt_beta_interval']
					self.optimal_swap_acceptance = vals['optimal_swap_acceptance']
					self.min_beta_adaptation_factor = vals['min_beta_adaptation_factor']
					self.max_beta_adaptation_factor = vals['max_beta_adaptation_factor']
					self.adapt_beta_rate = vals['adapt_beta_rate']
					#Step size adaptation options
					self.adapt_last = vals['adapt_last']
					self.adapt_relstep_interval = vals['adapt_relstep_interval']
					self.optimal_step_acceptance = vals['optimal_step_acceptance']
					self.min_adaptation_factor = vals['min_adaptation_factor']
					self.max_adaptation_factor = vals['max_adaptation_factor']
					self.adapt_relstep_rate = vals['adapt_relstep_rate']
	def swap(self,flag):
		for i in range(self.nchains-1,0,-1):
			#Try and swap chain i and i-1
			dE = self.c[i].energy_curr - self.c[i-1].energy_curr
			dB = self.c[i].beta -self.c[i-1].beta
			if flag==1:
				print("Trying to swap chains "+str(i+1)+" and "+str(i))
				print("Energy "+str(i)+": "+str(self.c[i].energy_curr))
				print("Energy "+str(i-1)+": "+str(self.c[i-1].energy_curr))
				print("dE: "+str(dE)+" dB: "+str(dB))
				print("h: "+str(math.log(self.r_swap[self.swap_counter][i-1]))+" dB*dE: "+str(dB*dE))
			if math.log(self.r_swap[self.swap_counter][i-1]) < dB*dE: #random.uniform(0,1)
				#swap chains
				energy_tmp = self.c[i].energy_curr
				self.c[i].energy_curr = self.c[i-1].energy_curr
				self.c[i-1].energy_curr = energy_tmp
				param_tmp = self.c[i].current
				self.c[i].current = self.c[i-1].current
				self.c[i-1].current = param_tmp
				self.swap_history[self.swap_counter][i-1] = 1
				if flag==1:
					print("swap accepted \n")
					print("Parameter set for chain "+str(i+1)+ "is ")
					print([x for x in self.c[i].current.x])
			else:
				self.swap_history[self.swap_counter][i-1] = 0
				if flag==1:
					print("swap rejected \n")
	def adapt_step_size(self,flag):
		num_chains = len(self.c)
		if flag==1:
			print('Adapting relative step sizes')
		for chain_idx in range(0,num_chains):
			chain = self.c[chain_idx]
			step_acceptance = copy.copy([x[chain_idx] for x in self.pt_step_acceptance[self.swap_counter+1-self.adapt_relstep_interval:self.swap_counter+1]])
			step_acceptance_rate = float(sum(step_acceptance))/(float(self.adapt_relstep_interval)*self.nstep)
			old_relstep = copy.copy(chain.relstep)
			adaptation_factor = (float(step_acceptance_rate/self.optimal_step_acceptance))**self.adapt_relstep_rate
			adaptation_factor = min(max(self.min_adaptation_factor,adaptation_factor),self.max_adaptation_factor)
			#Don't let higher temperature chains have smaller step sizes
			tmp1 = [x*adaptation_factor for x in old_relstep] #Proposed new step size
			if chain_idx>0:
				tmp2 = self.c[chain_idx-1].relstep #Step size of the immediate cooler chain
				#Take the max of the two for each list element
				self.c[chain_idx].relstep = [max(tmp2[i],tmp1[i]) for i in range(0,len(tmp1))]
			else:
				self.c[chain_idx].relstep = copy.copy(tmp1)
			if flag==1:
				print("\n Adapting step size, old step size: " + str(old_relstep) +" new step size: "+str(self.c[chain_idx].relstep )+"\n")
	def adapt_beta(self,flag):
		num_chains = len(self.c)
		if flag==1:
			print('Adapting chain temperatures')
		for chain_idx in range(1,num_chains): 
			#chain 1 has fixed temperature. adjust chains in order of coolest to hottest. Whenever a temperature changes, we proportionally increase the temperature of the hotter chains as well.
			chain = self.c[chain_idx]
			swap_acceptance = [x[chain_idx-1] for x in self.swap_history[self.swap_counter+1-self.adapt_beta_interval:self.swap_counter+1]]
			chain_swap_acceptance_rate = float(sum(swap_acceptance))/float(self.adapt_beta_interval)
			old_beta = copy.copy(chain.beta)
			adaptation_factor = (float(chain_swap_acceptance_rate)/float(self.optimal_swap_acceptance))**self.adapt_beta_rate
			adaptation_factor = min(self.max_beta_adaptation_factor,max(self.min_beta_adaptation_factor,adaptation_factor))
			#Make sure beta doesn't get too close to its cooler neighbor
			adaptation_factor = max(adaptation_factor,2*self.c[chain_idx].beta/(self.c[chain_idx].beta+self.c[chain_idx-1].beta))
			#Multiply this chain and higher temperature chains by adaptation factor
			for chain_idx2 in range(chain_idx,num_chains):
				self.c[chain_idx2].beta = self.c[chain_idx2].beta/adaptation_factor
			if flag==1:
				print("Chain "+str(chain_idx+1)+ ":\told temperature: "+str(1.0/old_beta)+"\tnew temperature: "+str(1.0/self.c[chain_idx].beta))
	def run(self,*argv):
		flag =0
		for arg in argv:
			if arg=="verbose":
				flag = 1
		self.nswap = self.nswap+self.swap_counter
		self.pt_step_acceptance = [[0 for x in range(0,self.nchains)] for y in range(0,self.nswap)] 
		for i in range(self.swap_counter,self.nswap):
			print("Iteration: "+str(self.swap_counter)+"\n")
			t1 = time.clock()
			for j in range(0,self.nchains):
				self.c[j].run(self.nstep,'verbose') #MCMC steps		
				self.pt_step_acceptance[i][j] = self.c[j].steps_accepted_current_run
			self.swap(flag)
			#Update chains
			self.param_curr = [k.current for k in self.c]
			self.energy_curr =[k.energy_curr for k in self.c]
			self.param_chain.append(self.param_curr)
			self.energy_chain.append(self.energy_curr)
			#print self.c[2].beta
			for j in range(0,len(self.c)):
				print("Chain ",j+1," Energy: ",self.energy_curr[j])#,"\n"
				print("Parameter set for chain "+str(j+1)+ "is ")
				print([x for x in self.c[j].current.x])
			print("\n") 
			#Not allowing temperature and steps to adapt on the same iteration is a ptempest quirk that I am leaving in here for now. Probably can be removed.
			if (self.swap_counter+1)%self.adapt_relstep_interval==0 and (self.swap_counter+1)%self.adapt_beta_interval!=0 and self.swap_counter<self.adapt_last:
				self.adapt_step_size(flag)
			if (self.swap_counter+1)%self.adapt_beta_interval==0 and self.swap_counter<self.beta_adapt_last:
				self.adapt_beta(flag)
			self.swap_counter = i+1
			t2 = time.clock()-t1
			#print "Swap time:"+str(time.clock()-t1)
			self.swap_time.append(t2)
	def save_pt(self,basename="pt_",foldername=".",*args):#Default folder to save is the current folder.
		#If nothing is specified, save all the attributes of this chain as individual text files. Else just save the attributes listed in args(TODO).
		#Save lists or lists of lists in separate files named with the attribute name; Save scalars all together in one file; Don't save functions or instances.
		#print self.energy_chain
		attr = vars(self)
		f = open('miscellaneous_pt_scalars.txt','w')
		for key in attr.keys():
			#print key			
			val = attr[key]
			if isinstance(val,list):
				#Check list element type
				if all(isinstance(x, int) or isinstance(x, float) for x in val): 
					f1 = open(basename+key+'.txt','w+')
					f1.writelines([str(x)+"\n" for x in val])
					f1.close()
				elif all(isinstance(x, list) for x in val):
					list_el = val[0]
					if all(isinstance(x, int) or isinstance(x, float) for x in list_el): 
						f1 = open(basename+key+'.txt','w+')
						for y in val:
							f1.writelines([str(x)+"\t" for x in y])
							f1.writelines("\n")
						f1.close()
				elif key=="c":
					for i in range(0,self.nchains):
						self.c[i].save_chain("Chain_"+str(i+1)+"_")
			elif isinstance(val,int) or isinstance(val,float):	
				f.write(key+"\t"+str(val)+"\n")
		f.close()	
def randomwalk(s,itr,relstep): #TODO: INCLUDE THE RELATIVE STEP SIZE HERE TOO
	dx = r_walk[itr]-0.5;
	return state(s.x+dx,s.prior,s.p1,s.p2)
def gaussian_proposal(s,itr,relstep,r_walk):
	num_par = len(s.x)
	loc = copy.copy(s.x)
	for i in range(0,num_par):
		dx = r_walk[itr][i]*relstep[i]
		loc[i] = loc[i] + dx
	return state_vector(loc,s.prior,s.p1,s.p2,s.simulate_function,s.likelihood_model,s.data)	
def energy_gaussian(par,simulate,data):
	y = simulate(par)
def loglikelihood_gaussian(par,simulate,data):
	y = simulate(par)
	numerator = [x**2 for x in (y-data['mean'])]
	denominator = [2*(x**2) for x in data['stdev']]
	loglikelihood = sum([numerator[i]/denominator[i] for i in range(0,len(y))])
	return -loglikelihood
