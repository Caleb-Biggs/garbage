#include "metadata.h"


// Assumes the pointer being passed in was allocated in an arena and
// returns the metadata which should be located immediately before it
Metadata* metadata_get(void* data){
	return (Metadata*)data-1;
}