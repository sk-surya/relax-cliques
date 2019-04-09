# ----------------------------------------------
# PRIMAL - MAXIMUM CLIQUE PROBLEM
# THIS IS THE DUAL FORMULATION
# MASTER PROBLEM
# ----------------------------------------------

#ROWS : Each set represent a list of maximal independent sets that contains vertex j (sets_j1)
set SETS_J;									

#number of columns (dynamic)
param nSETS integer >= 0;					

#COLUMNS : Each set represent a maximal independent set that holds a list of vertices (y1, y2, etc)
set MI_SETS; 								

let nSETS := 0;
set M_SETS:= 1..nSETS;


set EDGES;
set NODES;

param a{SETS_J, MI_SETS} integer >= 0;
param z{SETS_J, M_SETS} default 0;

param e{EDGES, NODES};
var y{M_SETS} integer >= 0;

minimize Independence: 
	sum {j in M_SETS} y[j];

subj to Sets_Containing_j{i in SETS_J}:
	sum {j in M_SETS} z[i,j] * y[j] >= 1;



	
# ----------------------------------------------
# CGSP - MAXIMUM WEIGHT INDEPENDENT SET PROBLEM
# ----------------------------------------------

param d{SETS_J} default 0.0;
var x{SETS_J} binary;

maximize I_Set_Weight:
	sum {i in SETS_J} d[i] * x[i];			#d is the vector of dual prices of the MP
	
subj to Edge_Constraints{i in EDGES}:
	#{j in NODES : j = 'node1'} x[e[i,j]] + {k in NODES : k = 'node2'}x[e[i,k]] <= 1;
	x[e[i,'node1']] + x[e[i,'node2']] <= 1;