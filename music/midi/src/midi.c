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
	//determine the necessary size of the variable length quantity
	int count = 1;
	while(val2>>=1){
		count++;
	}

	//determine the number of sets of 7 there are
	size_t size = ceil(count / 7.0);
	char* var = malloc(sizeof(char) * size);
	if(_size){
		(*_size) = size;
	}

	for(int i = 0; i < size; ++i){
		//zero out the MSB
		var[size - i - 1] = val & 0x7F;
		if(i){
			//if this is not the lowest byte
			//then set the MSB to 1
			var[size - i - 1] |= 0x80;
		}
		//do next set of 7
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
void new_midi(struct Midi* midi) {
	midi->chunk_count = 0;
	midi->chunks = malloc(sizeof(struct MidiChunk*) * midi->chunk_count);
	midi->header = NULL;
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

//{{{ midi_add_chunk
struct MidiChunk* midi_add_chunk(struct Midi* midi){
	midi->chunk_count++;
	midi->chunks = realloc(midi->chunks, sizeof(struct MidiChunk*) * midi->chunk_count);

	struct MidiChunk* chunk = malloc(sizeof(struct MidiChunk));
	midi->chunks[midi->chunk_count - 1] = chunk;
	return chunk;
}
//}}}

//{{{ midi_add_header
struct MidiHeaderChunk* midi_add_header(struct Midi* midi, uint16_t format, uint16_t tracks, uint16_t division){
	if(midi->header){
		//some error
	}
	struct MidiChunk* chunk = midi_add_chunk(midi);
	new_midichunk(chunk, CHUNK_HEADER);
	struct MidiHeaderChunk* header = malloc(sizeof(struct MidiHeaderChunk));
	new_midi_header(header, HEADER_LEN, format, tracks, division);
	chunk->chunk = header;
	midi->header = header;
	return header;
}
//}}}

//{{{ midi_add_track
struct MidiTrackChunk* midi_add_track(struct Midi* midi){
	struct MidiChunk* chunk = midi_add_chunk(midi);
	new_midichunk(chunk, CHUNK_TRACK);
	struct MidiTrackChunk* track = malloc(sizeof(struct MidiTrackChunk));
	new_midi_track(track);
	chunk->chunk = track;
	return track;
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
void new_midi_track(struct MidiTrackChunk* track){
	track->event_count = 0;

	track->events = malloc(sizeof(struct MidiEvent*) * track ->event_count);
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

//{{{ track_add_event
struct MidiEvent* track_add_event(struct MidiTrackChunk* track){
	track->event_count++;
	track->events = realloc(track->events, sizeof(struct MidiEvent*) * track->event_count);

	struct MidiEvent* event = malloc(sizeof(struct MidiEvent));
	track->events[track->event_count - 1] = event;
	return event;
}
//}}}

//{{{ track_add_event_full
struct MidiEvent* track_add_event_full(struct MidiTrackChunk* track, uint32_t delta_time, char* event_data, size_t event_data_len){
	struct MidiEvent* event = track_add_event(track);
	new_midi_event(event, delta_time, event_data, event_data_len);
	return event;
}
//}}}

//{{{ track_add_event_existing
void track_add_event_existing(struct MidiTrackChunk* track, struct MidiEvent* event){
	track->event_count++;
	track->events = realloc(track->events, sizeof(struct MidiEvent*) * track->event_count);

	track->events[track->event_count - 1] = event;
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

//{{{ write_midi
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
				fwrite(time, sizeof(char), time_size, f);
				fwrite(track->events[i]->event, sizeof(char), track->events[i]->event_len, f);
				free(time);
			}
		}
	}
}
//}}}

//{{{ read_midi
struct Midi* read_midi(FILE* f){	
	return NULL;
}
//}}}

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
	new_midi(m);
	//mode 2. 2 tracks. 120 ticks/quarternote
	midi_add_header(m, 2, 2, 120);
	//let's make a scale!
	struct MidiTrackChunk* track1 = midi_add_track(m);
	//{{{ Ascending ionian scale trombone: c4-c5
	//set instrument = trombone
	char instrument_event[2] = {0xC0, 58};
	track_add_event_full(track1, 0, instrument_event, 2);
	
	//noteOn, ch0, middle_c, mf
	char ev[3] = {0x90, 0x3C, 0x40};
	track_add_event_full(track1, 10, ev, 3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120, ev, 3);
	ev[0] = 0x90;
	ev[1] = 62;
	track_add_event_full(track1, 10, ev, 3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);
	ev[0] = 0x90;
	ev[1] = 64;
	track_add_event_full(track1, 10,ev,3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);
	ev[0] = 0x90;
	ev[1] = 65;
	track_add_event_full(track1, 10,ev,3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);
	ev[0] = 0x90;
	ev[1] = 67;
	track_add_event_full(track1, 10,ev,3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);
	ev[0] = 0x90;
	ev[1] = 69;
	track_add_event_full(track1, 10,ev,3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);
	ev[0] = 0x90;
	ev[1] = 71;
	track_add_event_full(track1, 10,ev,3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);
	ev[0] = 0x90;
	ev[1] = 72;
	track_add_event_full(track1, 10,ev,3);
	ev[0] = 0x80;
	track_add_event_full(track1, 120,ev,3);

	ev[0] = 0xFF;
	ev[1] = 0x2F;
	ev[2] = 0x00;
	track_add_event_full(track1, 0, ev, 3);
	//}}}

	//and the descending version on a second track!
	struct MidiTrackChunk* track2 = midi_add_track(m);
	//{{{ Descending ionian scale piano: c5-c4
	char ev2[3] = {0x91, 72, 0x40};
	track_add_event_full(track2, 10, ev2, 3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120, ev2, 3);
	ev2[0] = 0x91;
	ev2[1] = 71;
	track_add_event_full(track2, 10, ev2, 3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);
	ev2[0] = 0x91;
	ev2[1] = 69;
	track_add_event_full(track2, 10,ev2,3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);
	ev2[0] = 0x91;
	ev2[1] = 67;
	track_add_event_full(track2, 10,ev2,3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);
	ev2[0] = 0x91;
	ev2[1] = 65;
	track_add_event_full(track2, 10,ev2,3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);
	ev2[0] = 0x91;
	ev2[1] = 64;
	track_add_event_full(track2, 10,ev2,3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);
	ev2[0] = 0x91;
	ev2[1] = 62;
	track_add_event_full(track2, 10,ev2,3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);
	ev2[0] = 0x91;
	ev2[1] = 60;
	track_add_event_full(track2, 10,ev2,3);
	ev2[0] = 0x81;
	track_add_event_full(track2, 120,ev2,3);

	ev2[0] = 0xFF;
	ev2[1] = 0x2F;
	ev2[2] = 0x00;
	track_add_event_full(track2, 0, ev2, 3);
	//}}}

	FILE* f = fopen("test.mid", "wb");
	write_midi(m, f);
	fclose(f);
	free_midi(m);
	free(m);
	printf("Wrote to test.mid\n");
}
