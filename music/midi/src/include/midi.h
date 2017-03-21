#ifndef MIDI_H
#define MIDI_H 

#include <stdint.h>
#include <stdlib.h>

#define TYPE_LEN 4

///Converts a variable-length quantity to an integer
uint32_t varlen_to_int(char* var);

///Converts an integer to a variable-length quantity
char* int_to_varlen(uint32_t val, size_t* size);

struct MidiChunk {
	char type[TYPE_LEN];
	uint32_t length;
	char* data;
};

struct Midi {
	int chunk_len;
	struct MidiChunk* chunks;
};

#endif /* MIDI_H */
