#include "Sys/DLL.hpp"

namespace IW3SR
{
	PLUGIN void Initialize(DLL* entry)
	{
		entry->ID = "sr.reloader";
		entry->Name = "Reloader";
	}
}
