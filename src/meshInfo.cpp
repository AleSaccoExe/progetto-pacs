/*!	\file	meshInfo.cpp
	\brief	Specialization of some members of class meshInfo. */
	
#include "meshInfo.hpp"

namespace geometry
{
	//
	// Print in different format
	//
	
	// Specialization for triangular grids
	template<>
	void meshInfo<Triangle, MeshType::DATA>::print_inp(const string & filename) const
	{
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{			
			// Set number of significant figures
			file.precision(11);
			
			// Preamble
			file << this->connectivity.grid.getNumNodes() << " "
				 << this->connectivity.grid.getNumElems() << " " 
				 << "0 1 0" << endl;
				 
			// Print nodes
			auto nodes = this->connectivity.grid.getNodes();
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			auto elems = this->connectivity.grid.getElems();
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << 0				<< "  "
					 << "tri" 			<< "  "
					 << elem[0]+1		<< "  "
					 << elem[1]+1		<< "  "
					 << elem[2]+1		<< endl;
					 
			// Print quantity of information for each element
			file << "1 1" << endl;
			file << "QOI, -" << endl;
			for (auto elem : elems)
				file << elem.getId()+1 << " "
					 << getQuantityOfInformation(elem.getId()) << endl;
					 
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	void meshInfo<Quad, MeshType::DATA>::print_inp(const string & filename) const
	{
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{			
			// Set number of significant figures
			file.precision(11);
			
			// Preamble
			file << this->connectivity.grid.getNumNodes() << " "
				 << this->connectivity.grid.getNumElems() << " " 
				 << "0 0 0" << endl;
				 
			// Print nodes
			auto nodes = this->connectivity.grid.getNodes();
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			auto elems = this->connectivity.grid.getElems();
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << getQuantityOfInformation(elem.getId()) << "  "
					 << "quad" 			<< "  "
					 << elem[0]+1		<< "  "
					 << elem[1]+1		<< "  "
					 << elem[2]+1		<< "  "
					 << elem[3]+1		<< endl;
					 
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}

	template<>
	Real meshInfo<Triangle, MeshType::DATA>::computeMaxCos(const UInt & id1, const UInt & id2) const
	{
		Real max_cos=0.;
		auto elems = getElemsInvolvedInEdgeCollapsing(id1, id2);
			for(const auto & id_elem:elems)
			{
				auto elem = getCPointerToMesh()->getElem(id_elem);
				auto l1 = getCPointerToMesh()->getNode(elem[1]) - getCPointerToMesh()->getNode(elem[0]);
				auto l2 = getCPointerToMesh()->getNode(elem[2]) - getCPointerToMesh()->getNode(elem[1]);
				auto l3 = getCPointerToMesh()->getNode(elem[0]) - getCPointerToMesh()->getNode(elem[2]);
				Real cos0 = std::abs( -(l1*l3)/(l1.norm2()*l3.norm2()) );
				Real cos1 = std::abs( -(l1*l2)/(l1.norm2()*l2.norm2()) );
				Real cos2 = std::abs(-(l2*l3)/(l2.norm2()*l3.norm2()) );
				if(cos0>max_cos) 
					max_cos=cos0;
				if(cos1>max_cos)
					max_cos=cos1;
				if(cos2>max_cos)
					max_cos=cos2;
			}
		return max_cos;
	}

	template<>
	Real meshInfo<Triangle, MeshType::DATA>::computeMaxCos(const UInt & id_elem) const
	{
		Real max_cos=0.;
		auto elem = getCPointerToMesh()->getElem(id_elem);
		auto l1 = getCPointerToMesh()->getNode(elem[1]) - getCPointerToMesh()->getNode(elem[0]);
		auto l2 = getCPointerToMesh()->getNode(elem[2]) - getCPointerToMesh()->getNode(elem[1]);
		auto l3 = getCPointerToMesh()->getNode(elem[0]) - getCPointerToMesh()->getNode(elem[2]);
		Real cos0 = std::abs( -(l1*l3)/(l1.norm2()*l3.norm2()) );
		Real cos1 = std::abs( -(l1*l2)/(l1.norm2()*l2.norm2()) );
		Real cos2 = std::abs( -(l2*l3)/(l2.norm2()*l3.norm2()) );
		if(cos0>max_cos) 
			max_cos=cos0;
		if(cos1>max_cos)
			max_cos=cos1;
		if(cos2>max_cos)
			max_cos=cos2;
		return max_cos;
	}

}
