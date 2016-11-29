all:
	# reg pressure
	java -jar Mars4_5.jar outputs/BinaryTree.s 		>outputs/g1
	java -jar Mars4_5.jar outputs/BubbleSort.s 		>outputs/g2
	java -jar Mars4_5.jar outputs/Factorial.s 		>outputs/g3
	java -jar Mars4_5.jar outputs/LinearSearch.s 	>outputs/g4
	java -jar Mars4_5.jar outputs/LinkedList.s 		>outputs/g5
	java -jar Mars4_5.jar outputs/MoreThan4.s 		>outputs/g6
	java -jar Mars4_5.jar outputs/QuickSort.s 		>outputs/g7
	java -jar Mars4_5.jar outputs/TreeVisitor.s 	>outputs/g8

	# mine
	java -jar Mars4_5.jar output/BinaryTree.s 		>output/g1
	java -jar Mars4_5.jar output/BubbleSort.s 		>output/g2
	java -jar Mars4_5.jar output/Factorial.s 		>output/g3
	java -jar Mars4_5.jar output/LinearSearch.s 	>output/g4
	java -jar Mars4_5.jar output/LinkedList.s 		>output/g5
	java -jar Mars4_5.jar output/MoreThan4.s 		>output/g6
	java -jar Mars4_5.jar output/QuickSort.s 		>output/g7
	java -jar Mars4_5.jar output/TreeVisitor.s 		>output/g8


	# java -jar Mars4_5.jar BinaryTree.s 		>g1
	# java -jar Mars4_5.jar BubbleSort.s 		>g2
	# java -jar Mars4_5.jar Factorial.s 		>g3
	# java -jar Mars4_5.jar LinearSearch.s 	>g4
	# java -jar Mars4_5.jar LinkedList.s 		>g5
	# java -jar Mars4_5.jar MoreThan4.s 		>g6
	# java -jar Mars4_5.jar QuickSort.s 		>g7
	# java -jar Mars4_5.jar TreeVisitor.s		>g8

check:
	diff outputs/g1 output/g1
	diff outputs/g2 output/g2
	diff outputs/g3 output/g3
	diff outputs/g4 output/g4
	diff outputs/g5 output/g5
	# diff outputs/g6 output/g6
	diff outputs/g7 output/g7
	diff outputs/g8 output/g8