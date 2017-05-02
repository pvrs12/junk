#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LEN 19

struct cidr {
	uint32_t ip;
	//-1 signifies that this cidr is invalid
	int8_t bits;
};

int in_cidr(uint32_t ip, const struct cidr* cidr){
	if(cidr->bits == 0){
		return 1;
	}
	if(cidr->bits == -1){
		return 0;
	}
	uint8_t shift = sizeof(uint32_t) - cidr->bits;
	return !(ip >> shift ^ cidr->ip >> shift);
}

void print_cidr(const struct cidr* cidr){
#ifdef DEBUG
	printf("`%x/%d`", cidr->ip, cidr->bits);
	return;
#endif
	int i;
	uint32_t mask = 0xFF000000;
	for(i = 0; i < 4; ++i){
		int section = (cidr->ip & mask) >> ((3 - i) * 8);
		mask >>= 8;
		if(i){
			printf(".");	
		}
		printf("%d", section);
		
	}
	printf("/%d", cidr->bits);
}

int main(){
	int i,n;
	scanf("%d",&n);
	int filled = 0;
	struct cidr* cidrs = malloc(sizeof(struct cidr) * n);
	for(i = 0; i < n; ++i){
		char s[INPUT_LEN], ip_s[INPUT_LEN];
		scanf("%19s", s);

		const char* bits_s = strchr(s, '/');
		if(!bits_s){
			fprintf(stderr, "Invalid CIDR!\n");
			return 1;
		}
		bits_s += 1;
		strncpy(ip_s, s, (bits_s - s - 1));
		ip_s[bits_s - s - 1] = '\0';

		uint32_t ip  = 0;
		int8_t bits = atoi(bits_s);
		char* last = ip_s;
		char* remaining = strchr(last, '.');
		while(remaining) {
			//remove the '.'
			remaining += 1;
			char part[4];
			strncpy(part, last, (remaining - last - 1));
			part[remaining - last - 1] = '\0';
			ip |= atoi(part);
			ip <<= 8;
			last = remaining;
			remaining = strchr(last, '.');
		}
		ip |= atoi(last);
		int j, matched = 0;
		for(j = 0; j < filled; ++j){
			if(in_cidr(ip, &cidrs[j])){
				//if we have fewer bits, we should take precident
				if(cidrs[j].bits < bits){
					matched = 1;
					break;
				} 
			} 
		}
#ifdef DEBUG
		if(matched){
			printf("Did not add: `%x/%d`\n", ip, bits);
		}
#endif
		if(!matched){
			cidrs[filled].ip = ip;
			cidrs[filled].bits = bits;
#ifdef DEBUG
			printf("Added: ");
			print_cidr(&cidrs[filled]);
			printf("\n");
#endif
			filled++;
			//after adding a new cidr, see if it overlaps any that are within the list already
			struct cidr my_cidr = cidrs[filled - 1];
			for(j = 0; j < filled - 1; ++j){
				if(in_cidr(cidrs[j].ip, &my_cidr)){
					//we contain this one, so make it invalid
#ifdef DEBUG
					printf("Removed: ");
					print_cidr(&cidrs[j]);
					printf("\n");
#endif
					cidrs[j].bits = -1;
				}
			}
		}

	}
	for(i = 0; i < filled; ++i){
		if(cidrs[i].bits == -1){
			continue;
		}
		print_cidr(&cidrs[i]);
		printf("\n");
	}
	free(cidrs);
	cidrs = NULL;
}
