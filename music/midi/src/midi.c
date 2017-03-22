#include "include/midi.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//{{{ varlen_to_int
uint32_t varlen_to_int (char* var, size_t* size) {
	uint32_t val = 0;
	size_t _size = 0;
	while(1) {
		val |= (*var) & 0x7F;
		if(!((*var) >> 7)){
			break;
		}
		val <<= 7;
		var++;
		_size++;
	}
	if(size){
		(*size) = _size;
	}
	return val;	
}
//}}}

//{{{ int_to_varlen
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
//}}}

void new_midichunk(struct MidiChunk* chunk, char* type, uint32_t length){
	strncpy(chunk->type,type,TYPE_LEN);
	chunk->length = length;
}

void free_midichunk(struct MidiChunk* chunk){
}

void new_midi(struct Midi* midi, int tracks) {
	midi->chunk_len = tracks + 1;
	midi->chunks = malloc(sizeof(struct MidiChunk) * midi->chunk_len);
}

void free_midi(struct Midi* midi){
	for(int i = 0; i < midi->chunk_len;++i){
		free_midichunk(midi->chunks[i]);
	}
	free(midi->chunks);
}

void new_midi_header(struct MidiHeaderChunk* header, uint32_t length, uint16_t format, uint16_t tracks, uint16_t division){
	header->length = length;

	header->format = format;
	header->tracks = tracks;
	header->division = division;
}

void write_midi(struct Midi* midi, FILE* f) {
	for(uint32_t i = 0; i < midi->chunk_len; ++i){
		struct MidiChunk* chunk = midi->chunks[i];
		char type_c[4] = {'M','T','h','d'};
		fwrite(chunk->type, sizeof(char), 4, f);
		fwrite(&chunk->length, sizeof(uint32_t), 1, f);
		if(strncmp(chunk->type, type_c, 4) == 0){
			struct MidiHeaderChunk* header = (struct MidiHeaderChunk*) chunk->chunk;
			fwrite(&header->format, sizeof(uint16_t), 1, f);
			fwrite(&header->tracks, sizeof(uint16_t), 1, f);
			fwrite(&header->division, sizeof(uint16_t), 1, f);
		} else {
			struct MidiTrackChunk* track = (struct MidiTrackChunk*) chunk->chunk;
			size_t size;
			char* v = int_to_varlen(track->delta_time, &size);
			fwrite(v, sizeof(char), size, f);
			fwrite(v, sizeof(char), chunk->length - size, f);
		}
	}
}

int main(){
	size_t size;
	char* num = int_to_varlen(0x0FFFFFFF, &size);
	size_t read_size;
	int len = varlen_to_int(num, &read_size);
	for(int i=0;i<size;++i){
		printf("%hhx ",num[i]);
	}
	printf("\t|\t%x\n", len);
	free(num);

	FILE* f = fopen("test.mid", "wb");
	struct Midi* m = malloc(sizeof(struct Midi));
	new_midi(m, 1);
	m->chunks[0] = malloc(sizeof(struct MidiChunk));
	char type[4] = {'M','T','h','d'};
	new_midichunk(m->chunks[0], type, 6);
	struct MidiHeaderChunk* header = malloc(sizeof(struct MidiHeaderChunk));
	new_midi_header(header, 6, 0, 1, 0x0078);
	header->length = m->chunks[0]->length;
	m->chunks[0]->chunk = header;

	m->chunks[1] = malloc(sizeof(struct MidiChunk));
	//turn it into a track
	type[2]='r';
	type[3]='k';
	new_midichunk(m->chunks[1], type, <somefuckinsize>);
}
