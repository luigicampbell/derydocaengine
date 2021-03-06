#include "EnginePch.h"
#include "Files\Serializers\FileTypeSerializer.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

namespace DerydocaEngine::Files::Serializers {

	boost::uuids::uuid generateUuid()
	{
		boost::uuids::random_generator gen;
		boost::uuids::uuid id = gen();
		return id;
	}

}
