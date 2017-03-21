#include "include/midi.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

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

char* int_to_varlen(uint32_t val)	{
	
}

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
	/*for(int i=0;i<=0x7F;++i){*/
		/*char c[1];*/
		/*c[0] = i;*/
		/*uint32_t len = varlen_to_int(c);*/
		/*printf("var = %x\tlen = %x\n", i, len);*/
	/*}*/
	/*printf("===============\n");*/
	/*for(int i=0x80;i<=0xFF;++i){*/
		/*for(int j=0;j<=0x7F;++j){*/
			/*char c[2];*/
			/*c[0] = i;*/
			/*c[1] = j;*/
			/*uint32_t len = varlen_to_int(c);*/
			/*printf("var = %x%x\tlen = %x\n", i, j, len);*/
		/*}*/
	/*}*/
	char num[3] = {0xBD, 0x84, 0x40};
	int len = varlen_to_int(num);
	printf("%x %x %x\t%x\n", num[0], num[1], num[2], len);
}
