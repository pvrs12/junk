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
	//even if val == 0 we still need to allocate 1 byte
	//and we need to set the _size to 1 as well
	/*if(!val){*/
		/*char* c = malloc(sizeof(char) * 1);*/
		/*c[0] = 0x00;*/
		/*if(_size){*/
			/*(*_size) = 1;*/
		/*}*/
		/*return c;*/
	/*}*/

	uint32_t val2 = val;
	int count = 1;
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

//{{{ new_midichunk
void new_midichunk(struct MidiChunk* chunk, enum ChunkType type){
	if(type == CHUNK_HEADER){
		strncpy(chunk->type, "MThd", TYPE_LEN);
	} else {
		strncpy(chunk->type, "MTrk", TYPE_LEN);
	}
	chunk->type_e = type;

	chunk->chunk = NULL;
}
//}}}

//{{{ free_minichunk
void free_midichunk(struct MidiChunk* chunk){
	if(chunk->type_e == CHUNK_HEADER){
		struct MidiHeaderChunk* header = (struct MidiHeaderChunk*) chunk->chunk;
		free_midi_header(header);
		free(header);
	} else {
		struct MidiTrackChunk* track = (struct MidiTrackChunk*) chunk->chunk;
		free_midi_track(track);
		free(track);
	}
}
//}}}

//{{{ new_midi
void new_midi(struct Midi* midi, int tracks) {
	midi->chunk_count = tracks + 1;
	midi->chunks = malloc(sizeof(struct MidiChunk*) * midi->chunk_count);
}
//}}}

//{{{ free_midi
void free_midi(struct Midi* midi){
	for(int i = 0; i < midi->chunk_count;++i){
		free_midichunk(midi->chunks[i]);
		free(midi->chunks[i]);
	}
	free(midi->chunks);
}
//}}}

//{{{ new_midi_header
void new_midi_header(struct MidiHeaderChunk* header, uint32_t length, uint16_t format, uint16_t tracks, uint16_t division){
	header->length = length;

	header->format = format;
	header->tracks = tracks;
	header->division = division;
}
//}}}

//{{{ free_midi_header
void free_midi_header(struct MidiHeaderChunk* header){
	//nothing is allocated
}
//}}}

//{{{ new_midi_event
void new_midi_event(struct MidiEvent* event, uint32_t delta_time, char* ev, size_t event_length){
	event->delta_time = delta_time;
	event->event = malloc(sizeof(char) * event_length);
	event->event_len = event_length;
	strncpy(event->event, ev, event_length);
}
//}}}

//{{{ free_midi_event
void free_midi_event(struct MidiEvent* event){
	free(event);
}
//}}}

//{{{ new_midi_track
void new_midi_track(struct MidiTrackChunk* track, size_t event_count){
	track->event_count = event_count;

	track->events = malloc(sizeof(struct MidiEvent*) * event_count);
}
//}}}

//{{{ free_midi_track
void free_midi_track(struct MidiTrackChunk* track){
	for(int i = 0; i < track->event_count; ++i){
		free_midi_event(track->events[i]);
	}
	free(track->events);
}
//}}}

//{{{ track_length
size_t track_length(struct MidiTrackChunk* track){
	size_t s = 0;
	for(size_t i = 0; i < track->event_count; ++i){
		struct MidiEvent* e = track->events[i];
		size_t sz;
		free(int_to_varlen(e->delta_time, &sz));
		s += sz;
		s += e->event_len;
	}
	return s;
}
//}}}

//{{{ write_uint16_t
void write_uint16_t(uint16_t data, FILE* f){
	uint16_t d = htons(data);
	fwrite(&d, sizeof(uint16_t), 1, f);
}
//}}}

//{{{ write_uint32_t
void write_uint32_t(uint32_t data, FILE* f){
	uint32_t d = htonl(data);
	fwrite(&d, sizeof(uint32_t), 1, f);
}
//}}}

void write_midi(struct Midi* midi, FILE* f) {
	for(uint32_t i = 0; i < midi->chunk_count; ++i){
		struct MidiChunk* chunk = midi->chunks[i];
		fwrite(chunk->type, sizeof(char), TYPE_LEN, f);
		if(chunk->type_e == CHUNK_HEADER){
			struct MidiHeaderChunk* header = (struct MidiHeaderChunk*) chunk->chunk;
			write_uint32_t(header->length, f);
			write_uint16_t(header->format, f);
			write_uint16_t(header->tracks, f);
			write_uint16_t(header->division, f);
		} else {
			struct MidiTrackChunk* track = (struct MidiTrackChunk*) chunk->chunk;
			write_uint32_t(track_length(track), f);
			for(size_t i = 0; i < track->event_count; ++i){
				size_t time_size;
				char* time = int_to_varlen(track->events[i]->delta_time, &time_size);
				fwrite(&time, sizeof(char), time_size, f);
				fwrite(track->events[i]->event, sizeof(char), track->events[i]->event_len, f);
				free(time);
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
	new_midichunk(m->chunks[0], CHUNK_HEADER);
	struct MidiHeaderChunk* header = malloc(sizeof(struct MidiHeaderChunk));
	//6 bytes, type 0 (single track), 1 track, 120 ticks per 1/4 note
	new_midi_header(header, HEADER_LEN, 0, 1, 120);
	m->chunks[0]->chunk = header;

	m->chunks[1] = malloc(sizeof(struct MidiChunk));
	new_midichunk(m->chunks[1], CHUNK_TRACK);
	struct MidiTrackChunk* track = malloc(sizeof(struct MidiTrackChunk));
	//3 events; turn on C, turn off C, stop track
	new_midi_track(track, 3);
	m->chunks[1]->chunk = track;

	struct MidiEvent* note_on = malloc(sizeof(struct MidiEvent));
	//noteOn, ch0, middle_c, mf
	char ev[4] = {0x90, 0x3C, 0x40,0x00};
	new_midi_event(note_on, 0x01, ev, 4);
	track->events[0] = note_on;

	struct MidiEvent* note_off = malloc(sizeof(struct MidiEvent));
	ev[0] = 0x80;
	ev[1] = 0x3c;
	ev[2] = 0x40;
	new_midi_event(note_off, 0x01, ev, 4);
	track->events[1] = note_off;

	struct MidiEvent* track_end = malloc(sizeof(struct MidiEvent));
	char e[3] = {0xFF, 0x2F, 0x00};
	new_midi_event(track_end, 0x01, e, 3);
	track->events[2] = track_end;

	FILE* f = fopen("test.mid", "wb");
	write_midi(m, f);
	fclose(f);
	free_midi(m);
	free(m);
	printf("Wrote to test.mid\n");
}
