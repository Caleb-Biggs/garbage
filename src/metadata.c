#include "metadata.h"


// Assumes the pointer being passed in was allocated in an arena and
// returns the metadata which should be located immediately before it
Metadata* metadata_get(void* data){
	return (Metadata*)data-1;
}


void metadata_mark_delete(void* data){
	metadata_get(data)->mark = true;
}


// bool metadata_is_marked(void* data){
// 	return metadata_get(data)	
// }