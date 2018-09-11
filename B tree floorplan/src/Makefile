main:
	g++ -std=c++11 -O3 -o  ../bin/fp *.cpp
tc1_01:
	./../bin/fp "../testcase/n100.blocks" "../testcase/n100.nets" "../testcase/n100.pl" "../output/n100_0_1.floorplan" 0.1
tc1_015:
	./../bin/fp "../testcase/n100.blocks" "../testcase/n100.nets" "../testcase/n100.pl" "../output/n100_0_15.floorplan" 0.15
tc2_01:
	./../bin/fp "../testcase/n200.blocks" "../testcase/n200.nets" "../testcase/n200.pl" "../output/n200_0_1.floorplan" 0.1
tc2_015:
	./../bin/fp "../testcase/n200.blocks" "../testcase/n200.nets" "../testcase/n200.pl" "../output/n200_0_15.floorplan" 0.15
tc3_01:
	./../bin/fp "../testcase/n300.blocks" "../testcase/n300.nets" "../testcase/n300.pl" "../output/n300_0_1.floorplan" 0.1
tc3_015:
	./../bin/fp "../testcase/n300.blocks" "../testcase/n300.nets" "../testcase/n300.pl" "../output/n300_0_15.floorplan" 0.15
 
tc1_tight:
	./../bin/fp "../testcase/n100.blocks" "../testcase/n100.nets" "../testcase/n100.pl" "../output/n100_tight.floorplan" 0.0
tc2_tight:
	./../bin/fp "../testcase/n200.blocks" "../testcase/n200.nets" "../testcase/n200.pl" "../output/n200_tight.floorplan" 0.0
tc3_tight:
	./../bin/fp "../testcase/n300.blocks" "../testcase/n300.nets" "../testcase/n300.pl" "../output/n300_tight.floorplan" 0.0


ten_time_1_1:
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
	make tc1_01
ten_time_1_15:
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
	make tc1_015
ten_time_2_1:
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
	make tc2_01
ten_time_2_15:
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
	make tc2_015
ten_time_3_1:
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
	make tc3_01
ten_time_3_15:
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
	make tc3_015
clean:
	rm -rf *.o *.exe fp