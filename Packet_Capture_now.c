#define _CRT_SECURE_NO_WARNINGS
#define WIN32
#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <pcap\pcap.h>
#include <pcap.h>
#include <string.h>
#include <WinSock2.h>
#include <stdint.h>

#define IPHEADER 0x0800
#define ARPHEADER 0x0806
#define RARPHEADER 0x0835


typedef struct Ethernet_Header//檜渦喘 ④渦 掘褻羹
{
	u_char des[6];//熱褐濠 MAC 輿模
	u_char src[6];//歎褐濠 MAC 輿模
	short int ptype;//菴縑 釭螢 ぬ韃曖 Щ煎饜屬 謙盟(蕨:ARP/IP/RARP)
	   //IP ④渦陛 螃朝 唳辦 : 0x0800
	   //ARP ④渦陛 螃朝 唳辦 : 0x0806
	   //RARP ④渦陛 螃朝 唳辦 : 0x0835
}Ethernet_Header;//睡蒂 檜葷 摹樹(滌貲)

typedef struct ipaddress
{
	u_char ip1;
	u_char ip2;
	u_char ip3;
	u_char ip4;
}ip;

//IP Щ煎饜屬曖 ④渦蒂 盪濰й 掘褻羹 薑曖
typedef struct IPHeader
{
	u_char HeaderLength : 4;//④渦 望檜 *4
	u_char Version : 4;//IP v4 or IPv6
	u_char TypeOfService;//憮綠蝶 謙盟
	u_short TotalLength;//④渦 望檜 + 等檜攪 望檜/
	u_short ID;//Щ楚斜 試お曖 Identification
	u_short FlagOffset;//Ы楚斜 + Щ楚斜試お 螃Щ撢

	u_char TimeToLive;//TimeToL
	u_char Protocol;//Щ煎饜屬 謙盟(1. ICMP 2. IGMP 6. TCP 17:UDP;
	u_short checksum;
	ip SenderAddress;
	ip DestinationAddress;
	u_int Option_Padding;

	// 蹺陛
	unsigned short source_port;
	unsigned short dest_port;
}IPHeader;

typedef struct TCPHeader
{
	unsigned short source_port;
	unsigned short dest_port;
	unsigned int sequence;
	unsigned int acknowledge;
	unsigned char ns : 1;
	unsigned char reserved_part1 : 3;
	unsigned char data_offset : 4;
	unsigned char fin : 1;
	unsigned char syn : 1;
	unsigned char rst : 1;
	unsigned char psh : 1;
	unsigned char ack : 1;
	unsigned char urg : 1;
	unsigned char ecn : 1;
	unsigned char cwr : 1;
	unsigned short window;
	unsigned short checksum;
	unsigned short urgent_pointer;
}TCPHeader;

typedef struct udp_hdr

{
	unsigned short source_port; // Source port no.
	unsigned short dest_port; // Dest. port no.
	unsigned short udp_length; // Udp packet length
	unsigned short udp_checksum; // Udp checksum (optional)

} UDP_HDR;
typedef struct CheckSummer
{
	//0    2byte       2byte   32
	// [   4500   ][   003c   ] Version, HeaderLength, TypeOfService / TotalLength
	// [   11e5   ][   0000   ] Identification / Flag, FragmentOffset
	// [   8001   ][          ] TimeToLive, Protocol / HeaderChecksum
	// [   7c89   ][   19a4   ] Source Address
	// [   7c89   ][   19a3   ] Destination Address
	// 嬪 賅萇 璋濠曖 м檜 HeaderChecksum 高婁 偽擊 唳辦, ぬ韃擎 薑鼻檜棻.
	// 斜楛等 棻 渦ж賊 2037b 塭憮 2夥檜お 觼晦蒂 剩啪 脹棻.
	// 斜楚憮 萌 037b蒂 薯諼и 螃幗 Ы煎辦 高 2蒂 菴縑 渦и棻.
	//     037b
	//  +     2
	//  式式式式
	//     037d
	// 斜葬堅 釭憮 啗骯 唸婁 高(037d)蒂 爾熱 ⑽鷓煎 鏃и棻.
	// (1曖 爾熱 = 0擊 1煎, 1擊 0戲煎)
	// 037d = 0000 0011 0111 1101
	// 爾熱 = 1111 1100 1000 0010
	// 16霞 = fc82
	// 斜楝嘎煎 綠錶霞 睡碟縑朝 fc82陛 菟橫陛啪 脹棻.

	/*
	u_char = 1 byte
	u_short = 2 byte
	int = 4 byte
	*/
	//掘褻諦 蜃雖 彊雖虜 Version擊 ④渦 望檜 棻擠戲煎 嫡嬴撿 薑鼻瞳戲煎 ④渦 望檜諦 幗瞪檜 釭螞棻.
	/*
	u_char = 1 byte
	u_short = 2 byte
	int = 4 byte
	*/
	u_short part1;
	u_short part2;
	u_short part3;
	u_short part4;
	u_short part5;
	u_short checksum;
	u_short part6;
	u_short part7;
	u_short part8;
	u_short part9;

}CheckSummer;

// (蹺陛) HTTPHeader
typedef struct HTTPHeader
{
	uint16_t HTP[16];

}HTTPHeader;

typedef struct DNS
{
	u_char transaction_ID[2];
	u_char domain_name[40];

}domain;

void packet_handler(u_char* param, const struct pcap_pkthdr* h, const u_char* data); //ぬ韃擊 鼠и 瑞Щ 鼻鷓縑憮 檗堅 籀葬ж朝 л熱
void PrintHttpHeader(const uint8_t* packet); // Http④渦 轎溘 л熱 (蹺陛)
void PrintHexAscii(const u_char* buffer, unsigned int Psize); // 嬴蝶酈囀萄 滲�� (蹺陛)
void print_first(const struct pcap_pkthdr* h, Ethernet_Header* EH);
void print_protocol(Ethernet_Header* EH, short int type, IPHeader* IH, TCPHeader* TCP, CheckSummer* CS);

/*
瞪羲滲熱
*/
u_int sel = 0;
char str_[20] = { 0, };

void main()
{
	pcap_if_t* allDevice; //瓊嬴魚 蛤夥檜蝶蒂 LinkedList煎 僮堅, 斜 醞 羅 廓簞 螃粽薛お蒂 氬擊 滲熱 儅撩
	pcap_if_t* device; //Linked List曖 棻擠 螃粽薛お蒂 氬擊 奢除
	char errorMSG[256]; //縑楝 詭衛雖蒂 氬擊 滲熱 儅撩
	char counter = 0;

	pcap_t* pickedDev; //餌辨й 蛤夥檜蝶蒂 盪濰ж朝 滲熱

		   //1. 濰纂 匐儀 (瓊嬴魚 蛤夥檜蝶蒂 LinkedList煎 僮擠)
	if ((pcap_findalldevs(&allDevice, errorMSG)) == -1)//滲熱 儅撩衛縑朝 1 ん檣攪雖虜, pcap_findallDevice縑 噙朝勒 渦綰 ん檣お檜嘎煎 輿模煎 輿橫撿 л.
					  //pcap_if_t朝 int⑽鷓蒂 奩�納牉�, -1檜 釭螢 唳辦, 蛤夥檜蝶蒂 瓊雖 跤ц擊 唳辦檜棻.
		printf("濰纂 匐儀 螃盟");

	//2. 濰纂 轎溘
	int count = 0;
	for (device = allDevice; device != NULL; device = device->next)
		//dev縑 allDevice曖 羅 衛濛 輿模蒂 厥戲貊, dev曖 高檜 NULL(部)橾 唳辦 謙猿, dev朝 衙 for葆棻 棻擠 輿模高戲煎 瞪��
	{
		printf("旨  %d 廓 啻お錶觼 蘋萄收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n", count);
		printf("早 橫湘攪 薑爾 : %s 早\n", device->name);
		printf("早 橫湘攪 撲貲 : %s \n", device->description);
		printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭\n");
		count = count + 1;
	}

	//3. 啻お錶觼 蘋萄蒂 摹鷗ж堅 摹鷗脹 蛤夥檜蝶煎 熱餵й ぬ韃 唸薑ж晦
	printf("ぬ韃擊 熱餵й 啻お錶觼 蘋萄蒂 摹鷗 ж撮蹂 : ");
	device = allDevice;//蘋萄蒂 摹鷗ж雖 彊堅 斜傖 羅 廓簞 蘋萄煎 撲薑ц擠.

	int choice;
	scanf_s("%d", &choice);

	while (1) {
		printf("<в攪葭>\n");
		printf(" 1. ICMP\n 2. TCP\n 3. UDP\n 4. HTTP\n 5. FTP\n 6. DNS\n 7. ALL\n 8.IP煎 匐儀\n");
		printf(" >> ");
		scanf_s("%d", &sel);
		if (sel == 1 || sel == 2 || sel == 3 || sel == 4 || sel == 5 || sel == 6 || sel == 7 || sel == 8) {
			break;
		}
		else {
			printf("棻衛殮溘\n");
		}
	}

	for (count = 0; count < choice; count++)
	{
		device = device->next;
	}

	//啻お錶觼 濰纂蒂 翮堅, 熱餵й ぬ韃 曄擊 撲薑и棻.
	pickedDev = pcap_open_live(device->name, 65536, 0, 1000, errorMSG);
	//楠蘋萄曖 檜葷, 熱餵й ぬ韃 觼晦(譆渠 65536), Щ煎嘐蝶聽橫蝶賅萄(ぬ韃 熱餵 賅萄) 撲薑, ぬ韃 渠晦 衛除, 縑楝 薑爾蒂 盪濰й 奢除)

	//4. 楠蘋萄 葬蝶お 薑爾蒂 盪濰и 詭賅葬蒂 綠錶遽棻.
	pcap_freealldevs(allDevice);

	//5. 撲薑и 啻お錶觼 蘋萄縑憮 ぬ韃擊 鼠и 譫藥 й л熱蒂 虜菟堅 譫藥蒂 衛濛и棻.
	pcap_loop(pickedDev, 0, packet_handler, NULL);
}
// 等檜攪 轎溘
void print_data(const u_char* data) {
	printf("早  ----------------DATA--------------\n");
	printf("早\t%s \n", data);
}

// HTTP ④渦 轎溘
void PrintHttpHeader(const uint8_t* packet) {

	HTTPHeader* hh;
	hh = (HTTPHeader*)packet;
	printf("早  ---------------HTTP Header--------------\n");

	printf("早\t%s \n", packet);
	for (int i = 0; i < 16; i++) {
		//printf("%02x ", hh->HTP[i]);
		//PrintHexAscii(hh->HTP[i], sizeof(uint8_t));
	}
	printf("\n");
}

void PrintData(u_char* data, int Size)
{
	unsigned char a, line[17], c;
	int j;
	//loop over each character and print
	for (int i = 0; i < Size; i++)
	{
		c = data[i];
		//Print the hex value for every character , with a space
		printf(" %.2x", (unsigned int)c);
		//Add the character to data line
		a = (c >= 32 && c <= 128) ? (unsigned char)c : '.';
		line[i % 16] = a;
		//if last character of a line , then print the line - 16 characters in 1 line
		if ((i != 0 && (i + 1) % 16 == 0) || i == Size - 1)
		{
			line[i % 16 + 1] = '\0';
			//print a big gap of 10 characters between hex and characters
			printf("          ");
			//Print additional spaces for last lines which might be less than 16 characters in length
			for (j = strlen((const char*)line); j < 16; j++)
			{
				printf("   ");
			}
			printf("%s \n", line);
		}
	}
	printf("\n");
}

//嬴楚縑憮 餌辨й 熱 氈紫煙ぬ韃 с菟楝蒂 虜萇棻.
void packet_handler(u_char* param, const struct pcap_pkthdr* h, const u_char* data)
//檣濠 = だ塭嘐攪, ぬ韃 ④渦, ぬ韃 等檜攪(熱褐濠 MAC 輿模 睡碟 睡攪)
{
	/*
	 * unused variables
	 */
	(VOID)(param);
	(VOID)(data);

	//模蝶 檗擊 陽 陛絮撩擊 嬪п 鼻熱蒂 僥濠煎 夥羞棻.
	Ethernet_Header* EH = (Ethernet_Header*)data;//data 輿模縑 盪濰脹 14byte 等檜攪陛 掘褻羹 Ethernet_Header ⑽鷓煎 EH縑 盪濰脹棻.
	short int type = ntohs(EH->ptype);
	//EH->ptype擎 緊 縛蛤樹 ⑽衝擊 鏃ж嘎煎,
	//檜蒂 葬ぎ 縛蛤樹 ⑽衝戲煎 滲��(ntohs л熱)ж罹 type縑 盪濰и棻.
	IPHeader* IH = (struct IPHeader*)(data + 14); //薯橾 籀擠 14byte朝 檜渦喘 ④渦(Layer 2) 斜 嬪縑朝 IP④渦(20byte), 斜 嬪縑朝 TCP ④渦...
	TCPHeader* TCP = (struct TCPHeader*)(data + 34); // TCP ④渦 
	CheckSummer* CS = (struct CheckSummer*)(data + 14); //羹觼撰擊 盪濰 й 滲熱
	domain* dns = (struct DNS*) (data + 42);
	UDP_HDR* UDP = (struct UDP_HDR*)(data + IH->HeaderLength * 4 + sizeof(Ethernet_Header));




	char ip_comp[20];



	//1. ICMP 2. TCP 3. UDP 4. HTTP 5. FTP 6. ALL
	switch (sel) {
	case 1:
		if (IH->Protocol == IPPROTO_ICMP) {
			printf("Internet Control Message               \n");

			print_first(h, EH);
			print_protocol(EH, type, IH, TCP, CS);
			printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
		}
		break;
	case 2:
		if (IH->Protocol == IPPROTO_TCP) {
			print_first(h, EH);
			print_protocol(EH, type, IH, TCP, CS);

			printf("早  --------------------------------------------  \n");
			printf("早\t\t*[ TCP ④渦 ]*\t\t\n");
			printf("早\tSCR PORT : %d\n", ntohs(TCP->source_port));
			printf("早\tDEST PORT : %d\n", ntohs(TCP->dest_port));
			printf("早\tSeg : %u\n", ntohl(TCP->sequence));
			printf("早\tAck : %u\n", ntohl(TCP->acknowledge));
			printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
		}
		break;
	case 3:
		if (IH->Protocol == IPPROTO_UDP) {
			print_first(h, EH);
			print_protocol(EH, type, IH, TCP, CS);

			printf("早  --------------------------------------------  \n");
			printf("早\t\t*[ UDP ④渦 ]*\t\t\n");
			printf("早\tSCR PORT : %d\n", ntohs(UDP->source_port));
			printf("早\tDEST PORT : %d\n", ntohs(UDP->dest_port));
			printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
		}
		break;
	case 4:
		if (IH->Protocol == IPPROTO_TCP) {
			if (ntohs(TCP->source_port) == 80 || ntohs(TCP->dest_port) == 80 || ntohs(TCP->source_port) == 443 || ntohs(TCP->dest_port) == 443) {
				print_first(h, EH);
				print_protocol(EH, type, IH, TCP, CS);

				printf("早  --------------------------------------------  \n");
				printf("早\t\t*[ TCP ④渦 ]*\t\t\n");
				printf("早\tSCR PORT : %d\n", ntohs(TCP->source_port));
				printf("早\tDEST PORT : %d\n", ntohs(TCP->dest_port));
				printf("早\tSeg : %u\n", ntohl(TCP->sequence));
				printf("早\tAck : %u\n", ntohl(TCP->acknowledge));

				printf("早\tHTTP Щ煎饜屬 \n");
				uint8_t* packet = data + 34 + (IH->HeaderLength) * 4;
				printf("早  ---------------HTTP Header-------------- \n");
				printf("早\t%s \n", packet);
				printf("\n");
				printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
			}
		}
		break;
	case 5:
		if (IH->Protocol == IPPROTO_TCP) {
			if (ntohs(TCP->source_port) == 21 || ntohs(TCP->dest_port) == 21) {

				print_first(h, EH);
				print_protocol(EH, type, IH, TCP, CS);
				printf("早  --------------------------------------------  \n");
				printf("早\t\t*[ TCP ④渦 ]*\t\t\n");
				printf("早\tSCR PORT : %d\n", ntohs(TCP->source_port));
				printf("早\tDEST PORT : %d\n", ntohs(TCP->dest_port));
				printf("早\tSeg : %u\n", ntohl(TCP->sequence));
				printf("早\tAck : %u\n", ntohl(TCP->acknowledge));
				printf("早\tFTP Щ煎饜屬 \n");
				print_data(data + 34 + (IH->HeaderLength) * 4);
				printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
			}
		}
		break;
	case 6:
		if (ntohs(TCP->source_port) == 53 || ntohs(TCP->dest_port) == 53) {
			print_first(h, EH);
			print_protocol(EH, type, IH, TCP, CS);

			printf("早  --------------------------------------------  \n");
			printf("早\t\t*[ TCP ④渦 ]*\t\t\n");
			printf("早\tSCR PORT : %d\n", ntohs(TCP->source_port));
			printf("早\tDEST PORT : %d\n", ntohs(TCP->dest_port));
			printf("早\tSeg : %u\n", ntohl(TCP->sequence));
			printf("早\tAck : %u\n", ntohl(TCP->acknowledge));
			printf("早\tDNS Щ煎饜屬 \n");

			for (int i = 0; i < 40; i++) {
				if (dns->domain_name[i] > 60)
					printf("%c", dns->domain_name[i]);
				else if (dns->domain_name[i - 1] > 60)
					printf(".");
				if ((dns->domain_name[i - 2] == 'o' || dns->domain_name[i - 2] == 'k') && (dns->domain_name[i - 1] == 'm' || dns->domain_name[i - 1] == 'r') && dns->domain_name[i] < 60)
					break;
			}
			//printf("%s\n", dns->domain_name);
			printf("\n");
			printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
		}
		break;
	case 7:
		print_first(h, EH);
		print_protocol(EH, type, IH, TCP, CS);

		switch (IH->Protocol) {
		case IPPROTO_ICMP:
			printf("Internet Control Message               \n");
		case IPPROTO_IGMP:
			printf("Internet Group Management              \n");
			break;
		case IPPROTO_TCP:
			printf("Transmission Control(TCP)              \n");
			printf("早  --------------------------------------------  \n");
			printf("早\t\t*[ TCP ④渦 ]*\t\t\n");
			printf("早\tSCR PORT : %d\n", ntohs(TCP->source_port));
			printf("早\tDEST PORT : %d\n", ntohs(TCP->dest_port));
			printf("早\tSeg : %u\n", ntohl(TCP->sequence));
			printf("早\tAck : %u\n", ntohl(TCP->acknowledge));

			if (ntohs(TCP->source_port) == 21 || ntohs(TCP->dest_port) == 21) {
				printf("早\tFTP Щ煎饜屬 \n");
				print_data(data + 34 + (IH->HeaderLength) * 4);
			}

			if (ntohs(TCP->source_port) == 80 || ntohs(TCP->dest_port) == 80) {
				printf("早\tHTTP Щ煎饜屬 \n");
				PrintHttpHeader(data + 34 + (IH->HeaderLength) * 4);
			}
			break;
		case IPPROTO_PUP:
			printf("PUP                                    \n");
			break;
		case IPPROTO_UDP:
			printf("UDP                    \n");
			break;
		case IPPROTO_IDP:
			printf("XNS IDP                                \n");
			break;
		case IPPROTO_PIM:
			printf("Independent Multicast                  \n");
			break;
		case IPPROTO_RAW:
			printf("Raw IP Packets                         \n");
			break;
		default:
			printf("Unknown                                \n");
		}
		printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");

		break;
	case 8:
		if (str_[0] == 0) {
			printf("\nIP 殮溘 >> ");
			scanf_s("%s", str_);
			sprintf(ip_comp, "%d.%d.%d.%d", IH->SenderAddress.ip1, IH->SenderAddress.ip2, IH->SenderAddress.ip3, IH->SenderAddress.ip4);
		}
		if (strcmp(str_, ip_comp) == 0) {
			print_first(h, EH);
			print_protocol(EH, type, IH, TCP, CS);
			printf("曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
		}
		break;
	default:
		break;
	}
}

void print_protocol(Ethernet_Header* EH, short int type, IPHeader* IH, TCPHeader* TCP, CheckSummer* CS) {

	printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n");
	printf("早\t\t*[ Ethernet ④渦 ]*\t\t\n");
	printf("早\tSrc MAC : %02x-%02x-%02x-%02x-%02x-%02x\n", EH->src[0], EH->src[1], EH->src[2], EH->src[3], EH->src[4], EH->src[5]);//歎褐濠 MAC
	printf("早\tDst MAC : %02x-%02x-%02x-%02x-%02x-%02x\n", EH->des[0], EH->des[1], EH->des[2], EH->des[3], EH->des[4], EH->des[5]);//熱褐濠 MAC
	printf("早----------------------------------------------------------------------------------------------\n");

	//僭葬 啗類擎 01010101檜嘎煎 等檜攪 濠腦晦朝 寰п紫 脾.
	//④渦陛 稱朝 Layer2檣 等檜攪葭觼 啗類睡攪 濠腦賊 脾.

	if (type == IPHEADER)
	{
		printf("早\t\t*[ IP ④渦 ]*\n");
		printf("早\tProtocol : IP\n");
		int partSum = ntohs(CS->part1) + ntohs(CS->part2) + ntohs(CS->part3) + ntohs(CS->part4) + ntohs(CS->part5) + ntohs(CS->part6) + ntohs(CS->part7) + ntohs(CS->part8) + ntohs(CS->part9);
		u_short Bit = partSum >> 16;
		printf("早\tだお м : %08x\n", partSum);
		// printf("早\t4蘊 檜翕 : %08x\n", Bit);
		partSum = partSum - (Bit * 65536);
		// printf("早\t剩曹匙 渦и だお м : %04x\n", partSum + Bit);
		// printf("早\t爾熱 鏃ж晦 : %04x\n", (u_short)~(partSum + Bit));
		printf("早\t羹觼撰 : %04x\n", ntohs(CS->checksum));
		if (ntohs(CS->checksum) == (u_short)~(partSum + Bit))
			printf("早\t槳鼻腎雖 彊擎 薑鼻 ぬ韃殮棲棻.\n");
		else
			printf("早\t槳鼻脹 ぬ韃殮棲棻. 營 瞪歎 蹂羶擊 п撿 м棲棻.\n");
		printf("早\t幗瞪 : IPv%d\n", IH->Version);
		printf("早\t④渦 望檜 : %d\n", (IH->HeaderLength) * 4);
		printf("早\t憮綠蝶 謙盟 : %04x\n", IH->TypeOfService);
		printf("早\t瞪羹 觼晦 : %d\n", ntohs(IH->HeaderLength));//2 bytes 檜鼻 睡攪朝 鼠褻勒 菴餵橫撿 ж嘎煎 ntohsл熱蒂 賦憮 菴餵朝棻.
		printf("早\tЩ楚斜試お 螃Щ撢 : %d[byte]\n", (0x1FFF & ntohs(IH->FlagOffset) * 8));
		printf("早\tTTL : %d\n", IH->TimeToLive);
		//  printf("早\t羹觼撰 : %04x\n", ntohs(IH->checksum));//蕨) 0x145F
		printf("早\t轎嫦 IP 輿模 : %d.%d.%d.%d\n", IH->SenderAddress.ip1, IH->SenderAddress.ip2, IH->SenderAddress.ip3, IH->SenderAddress.ip4);
		printf("早\t紫雜 IP 輿模 : %d.%d.%d.%d\n", IH->DestinationAddress.ip1, IH->DestinationAddress.ip2, IH->DestinationAddress.ip3, IH->DestinationAddress.ip4);
		//   printf("早\t褫暮/ぬ註 : %d\n", IH->Option_Padding);
		printf("\n");

		// printf("早\t撮睡 Щ煎饜屬 : "/*, IH->Protocol*/);
		
		int iphdrlen = 0;
		iphdrlen = IH->HeaderLength * 64;

		PrintData((u_char*)IH, iphdrlen);
	}
	else if (type == ARPHEADER)
	{
		printf("早\tProtocol : ARP\n");
	}
	else if (type == RARPHEADER)
		printf("早\tProtocol : RARP\n");
}

void print_first(const struct pcap_pkthdr* h, Ethernet_Header* EH) {
	struct tm ltime;
	char timestr[16];
	time_t local_tv_sec;

	/* convert the timestamp to readable format */
	local_tv_sec = h->ts.tv_sec;
	localtime_s(&ltime, &local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", &ltime);
	printf("Time: %s,%.6d len:%d\n", timestr, h->ts.tv_usec, h->len);

	printf("Next Packet : %04x\n", EH->ptype);
}