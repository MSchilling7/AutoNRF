To compile use make. Be sure that in addition to the ROOT Libary this folder is also included in the LD_LIBRARY_PATH.

#1 export LD_LIBRARY_PATH=.:$ROOTSYS/installation/directory/lib/root/
#2 make

Use optional 'make run'. Then the right path is set automatically for this evaluation.

Usage: AutoNRF [OPTION...] File
Evaluating a NRF Experiment with ROOT 
									
       The Data should be in a propper way.				
       There is no problems for , or ; for inbetween.			
									
    Energy   ΔEnergy   FWHM   EFWHM   Area   ΔArea   TailL   ΔTailL	
									
	Parameterfile							
									
	#1	Path to Peakfile					
	#2	Time of Measurement					
	#3	Fitting-Mode (0=Knoll, 1=a4-Model, 2=Jäckel, 3=All)	
	#4	Starting Value for a0					
	#5	Starting Value for a1					
	#6	Starting Value for a2					
	#7	Starting Value for a3					
	#8	Starting Value for a4					
	#9	Starting Value for a5					
									
		NEXT LINE FOR ANOTHER FIT				

Report bugs to <mschilling@ikp.tu-darmstadt.de>.


