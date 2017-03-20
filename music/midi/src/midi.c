#include "include/midi.h"

#include <malloc.h>
#include <string.h>

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
