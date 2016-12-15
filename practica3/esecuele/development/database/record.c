#include "record.h"

struct record_ {
	int ncols;
	void** values;
	long next;
};

record_t* record_create(void** values, int ncols, long next) {
	record_t* record;

	if( values == NULL || ncols < 0 || next < 0) return NULL;

	record = malloc(sizeof(record_t));
	if(record == NULL) return NULL;

	record->ncols = ncols;
	record->values = values;
	record->next = next;

	return record; 
}

void* record_get(record_t* record, int n) {
	if(record == NULL || n < 0) return NULL;

	return record->values[n];
}

long record_next(record_t* record) {
	if(record == NULL) return -1;

	return record->next;
}

void record_free(record_t* record) {
	int i = 0;
	
	while(record->values[i]){
		free(record->values[i];
	}
	free(record->values);
	free(record);

	return;
}
