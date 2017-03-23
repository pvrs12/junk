#include "include/midi.h"

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//for htonl 
#include <netinet/in.h>

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
	char type_c[4] = {'M','T','h','d'};
	if(strncmp(chunk->type, type_c, 4) == 0){
		struct MidiHeaderChunk* header = (struct MidiHeaderChunk*) chunk->chunk;
		free_midi_header(header);
		free(header);
	} else {
		struct MidiTrackChunk* track = (struct MidiTrackChunk*) chunk->chunk;
		free_midi_track(track);
		free(track);
	}
}

void new_midi(struct Midi* midi, int tracks) {
	midi->chunk_len = tracks + 1;
	midi->chunks = malloc(sizeof(struct MidiChunk*) * midi->chunk_len);
}

void free_midi(struct Midi* midi){
	for(int i = 0; i < midi->chunk_len;++i){
		free_midichunk(midi->chunks[i]);
		free(midi->chunks[i]);
	}
	free(midi->chunks);
}

void new_midi_header(struct MidiHeaderChunk* header, uint32_t length, uint16_t format, uint16_t tracks, uint16_t division){
	header->length = length;

	header->format = format;
	header->tracks = tracks;
	header->division = division;
}

void free_midi_header(struct MidiHeaderChunk* header){
	//nothing is allocated
}

void new_midi_event(struct MidiEvent* event, uint32_t delta_time, char* ev){
	event->delta_time = delta_time;
	size_t s = strnlen(ev, MAX_EVENT_LEN);
	if(!s || s == MAX_EVENT_LEN){
		// some error
		event->event_len = 0;
	} else {
		event->event = malloc(sizeof(char) * s);
		event->event_len = s;
		strncpy(event->event, ev, s);
	}	
}

void free_midi_event(struct MidiEvent* event){
	free(event);
}

void new_midi_track(struct MidiTrackChunk* track, uint32_t length, size_t event_count){
	track->length = length;
	track->event_count = event_count;

	track->events = malloc(sizeof(struct MidiEvent*) * event_count);
}

void free_midi_track(struct MidiTrackChunk* track){
	for(int i = 0; i < track->event_count; ++i){
		free_midi_event(track->events[i]);
	}
	free(track->events);
}

void write_uint16_t(uint16_t data, FILE* f){
	uint16_t d = htons(data);
	fwrite(&d, sizeof(uint16_t), 1, f);
}

void write_uint32_t(uint32_t data, FILE* f){
	uint32_t d = htonl(data);
	fwrite(&d, sizeof(uint32_t), 1, f);
}

void write_midi(struct Midi* midi, FILE* f) {
	for(uint32_t i = 0; i < midi->chunk_len; ++i){
		struct MidiChunk* chunk = midi->chunks[i];
		fwrite(chunk->type, sizeof(char), 4, f);
		write_uint32_t(chunk->length, f);
		char type_c[4] = {'M','T','h','d'};
		if(strncmp(chunk->type, type_c, 4) == 0){
			struct MidiHeaderChunk* header = (struct MidiHeaderChunk*) chunk->chunk;
			write_uint16_t(header->format, f);
			write_uint16_t(header->tracks, f);
			write_uint16_t(header->division, f);
		} else {
			struct MidiTrackChunk* track = (struct MidiTrackChunk*) chunk->chunk;
			for(size_t i = 0; i < track->event_count; ++i){
				size_t time_size;
				char* time = int_to_varlen(track->events[i]->delta_time, &time_size);
				fwrite(&(time), sizeof(char), time_size, f);
				fwrite(track->events[i]->event, sizeof(char), track->events[i]->event_len, f);
			}
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

	struct Midi* m = malloc(sizeof(struct Midi));
	new_midi(m, 1);
	m->chunks[0] = malloc(sizeof(struct MidiChunk));
	char type[4] = {'M','T','h','d'};
	new_midichunk(m->chunks[0], type, 6);
	struct MidiHeaderChunk* header = malloc(sizeof(struct MidiHeaderChunk));
	//6 bytes, type 0 (single track), 1 track, 120 ticks per 1/4 note
	new_midi_header(header, 6, 0, 1, 0x0078);
	header->length = m->chunks[0]->length;
	m->chunks[0]->chunk = header;

	m->chunks[1] = malloc(sizeof(struct MidiChunk));
	//turn it into a track
	type[2]='r';
	type[3]='k';
	new_midichunk(m->chunks[1], type, 5);
	struct MidiTrackChunk* track = malloc(sizeof(struct MidiTrackChunk));
	track->length = m->chunks[0]->length;
	m->chunks[1]->chunk = track;
	//a single event long. it is the end of track event
	//<delta time>   <end of track>
	//00             FF 2F 00
	new_midi_track(track, 5, 1);

	struct MidiEvent* event = malloc(sizeof(struct MidiEvent));
	char e[3] = {0xFF, 0x2F, 0x00};
	new_midi_event(event, 0x00, e);
	track->events[0] = event;

	FILE* f = fopen("test.mid", "wb");
	write_midi(m, f);
	fclose(f);
	free_midi(m);
	free(m);
}
