# Tasks:

	# Make temp copy of target code
	cp $1 tmp/temp0.java

	# First Pass
	# MacroJAVA converted to MiniJAVA
	cd pass_1 && echo "Executing Pass 1" && ./P1 <../tmp/temp0.java >../tmp/temp1.java 
	cd ..

	# Second Pass
	# Typechecking MiniJAVA
	cd pass_2 && echo "Executing Pass 2" && java P2 <../tmp/temp1.java
	cd ..

	# Third Pass
	# MiniJAVA converted to MiniIR
	cd pass_3 && echo "Executing Pass 3" && java P3 <../tmp/temp1.java >../tmp/temp3.miniIR 
	cd ..

	# Fourth Pass
	# MiniIR converted to MicroIR
	cd pass_4 && echo "Executing Pass 4" && java P4 <../tmp/temp3.miniIR >../tmp/temp4.microIR 
	cd ..

	# Fifth Pass
	# MicroIR converted to MiniRA
	cd pass_5 && echo "Executing Pass 5" && java P5 <../tmp/temp4.microIR >../tmp/temp5.miniRA 
	cd ..

	# Sixth Pass
	# MiniRA converted to MIPS assembly
	cd pass_6 && echo "Executing Pass 6" && java P6 <../tmp/temp5.miniRA >../tmp/temp6.s 
	cd ..

	# Rename output file
	mv tmp/temp6.s tmp/output.s

	# Remove temporaries
	rm tmp/temp*