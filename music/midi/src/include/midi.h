#ifndef MIDI_H
#define MIDI_H 

#include <stdint.h>

#define TYPE_LEN 4

///Converts a Variable-length quantity to an integer
uint32_t varlen_to_int(char* var);

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
