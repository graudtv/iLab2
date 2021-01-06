#include <iostream>
#include "electronic_circuit.h"
#include "scan.h"

int main()
try {
	int opt;
	int opt_dump_incidence_matrix = 1;
	int opt_dump_nodes_and_components = 1;
	int opt_dump_currents = 1;

	Physics::ElectronicCircuit circuit;

	while (std::cin) {
		std::string line;
		int node1, node2;
		double R;
		double E = 0;

		std::getline(std::cin, line);
		scn::scanner scan(line);

		if (line.size() == 0)
			break;
		scan >> node1 >> scn::skip_spaces >> "--" >> scn::skip_spaces >> node2
			>> scn::skip_spaces >> "," >> scn::skip_spaces >> R;
		try {
			scan >> scn::skip_spaces >> scn::end_of_text;
			printf("resistor\n");
			circuit.add_component(node1, node2, Physics::make_resistor(R));
			continue;
		} catch (scn::scan_error& e) {}
		printf("voltage\n");
		scan >> ";" >> scn::skip_spaces >> E >> scn::skip_spaces
			>> "V" >> scn::skip_spaces >> scn::end_of_text;
		circuit.add_component(node1, node2, Physics::make_voltage_src(R, E)); // resistor is voltage source with E = 0
	}

	if (opt_dump_nodes_and_components) {
		printf("%zu\n", circuit.ncomponents());
		printf("%zu\n", circuit.max_node_idx());	
	}
	if (opt_dump_incidence_matrix)
		circuit.get_incidence_matrix().dump();
	if (opt_dump_currents) {
		for (int comp_idx = 0; comp_idx != circuit.ncomponents(); ++comp_idx) {
			auto comp_data = circuit.get_component_data(comp_idx);
			std::cout << comp_data.node1 << " -- " <<comp_data.node2 << ": "
				<< circuit.get_current(comp_idx) << " A\n";
		}
	}

	return 0;
} catch (scn::scan_error& e) {
	std::cerr << "error while parsing input data: " << e.what() << std::endl;
}