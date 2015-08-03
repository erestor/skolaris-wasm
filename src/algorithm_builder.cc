//----------------------------------------------------------------------------
// Author:		Martin Klemsa
//----------------------------------------------------------------------------
#include "algorithm_builder.h"
#include <algorithm/factory.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

unique_ptr<Algorithm::IAlgorithm> PluginAlgorithmBuilder::BuildAlgorithm(const string &jsonDefinition)
{
	ptree algPt;
	{
		stringstream s(jsonDefinition);
		json_parser::read_json(s, algPt);
	}
	return Algorithm::Create(algPt);
}
