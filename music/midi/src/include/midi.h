#ifndef MIDI_H
#define MIDI_H 

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define TYPE_LEN 4

///Converts a variable-length quantity to an integer
uint32_t varlen_to_int(char* var, size_t* size);

///Converts an integer to a variable-length quantity
char* int_to_varlen(uint32_t val, size_t* size);

struct MidiChunk {
	char type[TYPE_LEN];
	uint32_t length;
	void* chunk;
};

void new_midichunk(struct MidiChunk* chunk, char* type, uint32_t length);
void free_midichunk(struct MidiChunk* chunk);

struct Midi {
	uint32_t chunk_len;
	//array of MidiChunk*
	struct MidiChunk** chunks;
};

void new_midi(struct Midi* midi, int tracks);
void free_midi(struct Midi* midi);

struct MidiHeaderChunk {
	uint32_t length;

	uint16_t format;
	uint16_t tracks;
	uint16_t division;
};

void new_midi_header(struct MidiHeaderChunk* header, uint32_t length, uint16_t format, uint16_t tracks, uint16_t division);

struct MidiTrackChunk {
	uint32_t length;

	uint32_t delta_time;
	char* event;
};

void write_midi(struct Midi* m, FILE* f);

//www.personal.kent.edu/~sbirch/Music_Production/MP-II/MIDI/midi_file_format.htm

#endif /* MIDI_H */
