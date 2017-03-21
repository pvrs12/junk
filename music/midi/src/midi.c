#include "include/midi.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//vim:startfold
uint32_t varlen_to_int (char* var) {
	uint32_t val = 0;
	while(1) {
		val |= (*var) & 0x7F;
		if(!((*var) >> 7)){
			break;
		}
		val <<= 7;
		var++;
	}
	return val;	
}
//vim:endfold

//vim:startfold
char* int_to_varlen(uint32_t val, size_t* _size)	{
	uint32_t val2 = val;
	int count = 0;
	//determine the necessary size of the variable length quantity
	while(val2>>=1){
		count++;
	}

	size_t size = ceil(count / 7.0);
	char* var = malloc(sizeof(char) * size);
	if(_size){
		(*_size) = size;
	}

	for(int i = 0; i < size; ++i){
		var[size - i - 1] = val & 0x7F;
		if(i){
			var[size - i - 1] |= 0x80;
		}
		val >>= 7;
	}
	return var;
}
//vim:endfold

void new_midichunk(struct MidiChunk* chunk, char* type, uint32_t length, char* data){
	strncpy(chunk->type,type,TYPE_LEN);
	//@todo
	//convert length from variable into to real int
	chunk->length = length;
	chunk->data = malloc(sizeof(char) * chunk->length);
	memcpy(chunk->data, data, chunk->length);
}

void free_midichunk(struct MidiChunk* chunk){
	free(chunk -> data);
}

void new_midi(struct Midi* midi, int tracks) {
	midi->chunk_len = tracks + 1;
	midi->chunks = malloc(sizeof(struct MidiChunk) * midi->chunk_len);
}

void free_midi(struct Midi* midi){
	for(int i = 0; i < midi->chunk_len;++i){
		free_midichunk(&midi->chunks[i]);
	}
	free(midi->chunks);
}

#include <stdio.h>
int main(){
	size_t size;
	char* num = int_to_varlen(0x0FFFFFFF, &size);
	int len = varlen_to_int(num);
	for(int i=0;i<size;++i){
		printf("%hhx ",num[i]);
	}
	printf("\t|\t%x\n", len);
	free(num);
}
