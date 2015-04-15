#include <vector>
#include <map>
#include <string>
#include <memory>
#include <tuple>
#include <algorithm>
#include <climits>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <execinfo.h>
#include <pthread.h>
#include <cxxabi.h>
#include <wordexp.h>
#include <unistd.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <dlfcn.h>

#include "leakfinder.h"

static void ParseArgs(std::string cmd, bool &append_flag, std::string &f_file, int &cnt)
{
	wordexp_t result;
	cmd = "leakfinder.so " + cmd;
	if (wordexp(cmd.c_str(), &result, 0) != 0)
	{
		wordfree(&result);
		exit(1);
	}

	int c;
	while ((c = getopt(result.we_wordc, result.we_wordv, "haf:b:")) != -1) {
		switch (c) {
		case 'a':
			append_flag = 1;
			break;
		case 'f':
			f_file = optarg;
			break;
		case 'b':
			printf("%s\n", optarg);
			cnt = atoi(optarg);
			printf("%d\n", cnt);
			break;
		case 'h':
			fprintf(stderr, "####################\n");
			fprintf(stderr, "#### leakfinder ####\n");
			fprintf(stderr, "####################\n");
			fprintf(stderr, "Usage:\n");
			fprintf(stderr, "\tLEAKFINDER=\"-f <file> -a -b <num>\" LD_PRELOAD=leakfinder.so <pgm> ....\n");
			fprintf(stderr, "or:\n");
			fprintf(stderr, "\tgdb <pgm> ....\n");
			fprintf(stderr, "\tset environment LD_PRELOAD=leakfinder.so\n");
			fprintf(stderr, "\tstart\n");
			fprintf(stderr, "\n");
			fprintf(stderr, "Options:\n");
			fprintf(stderr, "-f <file>   Memory dump log to <file>. Default stderr.\n");
			fprintf(stderr, "-a          Append log to <file>. Default overwrite.\n");
			fprintf(stderr, "-b <num>    Break when <num> alloc is reached.\n");
			fprintf(stderr, "-h          This help\n");
			exit(0);
			break;
		case '?':
			if (optopt == 'f')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
			exit(0);

		default:
			abort();
		}
	}
	if (optind < (int)result.we_wordc) {
		printf ("Illegal argument(s) %s....\n", result.we_wordv[optind]);
		exit(1);
	}
	wordfree(&result);
}


	//////////////////////////////////////////////

struct CallInfo {
	std::string trace;

	// sono ricavati da this->trace
	struct Data {
		std::string file;
		std::string fun;
		//int         offset;
		size_t      addr;
		int         line;

		Data() { addr = 0; line = 0; }
	};

	void Dump(FILE *f, std::map<std::string, int> &addr2line) const {
		Data d = GetData(addr2line);

		if (!d.file.empty()) {
			if (d.line > 0)
				fprintf(f, "\t%s(%d): ", d.file.c_str(), d.line);
			else
				fprintf(f, "\t%s(): ", d.file.c_str());
			fprintf(f, "%s", d.fun.c_str());
		}
		fprintf(f, " - %s\n", trace.c_str());
	}

	Data GetData(std::map<std::string, int> &addr2line) const {
		Data ret;

		std::string module;

		size_t s = 0;
		size_t e = trace.find("(");
		if (e != std::string::npos) {
			module = trace.substr(s, e-s);
			s = e;
		}
		/*
		size_t s = 0;
		size_t e = trace.find("(");
		if (e != std::string::npos) {
			module = trace.substr(s, e-s);
			s = e;

			e = trace.find("+", s);
			if (e != std::string::npos) {

				ret.fun = trace.substr(s+1, e-(s+1));
				if (!ret.fun.empty()) {
					int st = 0;
					char *p = abi::__cxa_demangle(ret.fun.c_str(), nullptr, 0, &st);
					if (st == 0 && p != nullptr)
						ret.fun = p;
				}
				s = e;
				e = trace.find(")", s);

				if (e != std::string::npos) {
					std::string bb = trace.substr(s+1, e-(s+1));
					ret.offset = strtol(bb.c_str(), nullptr, 16);
				}
			}
		}

		e = trace.find("[", s);
		if (e != std::string::npos) {
			s = e;
			e = trace.find( "]", s);
			if (e != std::string::npos) {
				std::string addr = trace.substr(s+1, s+1-e);
				ret.addr = strtol(addr.c_str(), nullptr, 16);
			}
		}
		*/

		e = trace.find("[", s);
		if (e != std::string::npos) {
			s = e;
			e = trace.find( "]", s);
			if (e != std::string::npos) {
				std::string addr = trace.substr(s+1, s+1-e);
				ret.addr = strtol(addr.c_str(), nullptr, 16);
			}
		}

		char b[1024];
		sprintf(b, "addr2line -f -C -e '%s' 0x%lx", module.c_str(), ret.addr);
		fflush(stdout);
		std::map<std::string, int>::iterator itAddr2Line = addr2line.find(b);
		if (itAddr2Line == addr2line.end()) {
			unsetenv("LD_PRELOAD");
			FILE *f = popen(b, "r");
			char row[1024];
			if (fgets(row, sizeof(row), f)) {
				int i = strlen(row);
				if (i>0 && row[i-1] == '\n') row[i-1] = 0;
				ret.fun = row;
			}
			if (fgets(row, sizeof(row), f)) {
				int i = strlen(row);
				if (i>0 && row[i-1] == '\n') row[i-1] = 0;
				char *d = strrchr(row, ':');
				if (d) {
					*d = 0;
					ret.file = row;
					ret.line = atoi(d + 1);
				}
			}
			pclose(f);

			addr2line[b] = ret.line;
		}
		else 
			ret.line = addr2line[b];

		return ret;
	}
};

inline static bool operator < (const CallInfo &a, const CallInfo &b) { return a.trace < b.trace; }
inline static bool operator > (const CallInfo &a, const CallInfo &b) { return a.trace > b.trace; }

struct TraceInfo
{
	void *address;
	size_t size;
	pthread_t thread_id;
	int allocNumber;
	std::shared_ptr<std::vector<CallInfo>> stacktrace;

	TraceInfo(void *address, size_t size, std::shared_ptr<std::vector<CallInfo>> stacktrace, pthread_t thread_id, int allocNumber) 
	{
		this->address     = address;
		this->size        = size;
		this->stacktrace  = stacktrace;
		this->thread_id   = thread_id;
		this->allocNumber = allocNumber;
	}
	TraceInfo()
	{
		this->address = 0;
		this->size = 0;
		this->thread_id = 0;
		this->allocNumber = 0;
	}
	TraceInfo(const TraceInfo &r) {
		this->address     = r.address;
		this->size        = r.size;
		this->stacktrace  = r.stacktrace;
		this->thread_id   = r.thread_id;
		this->allocNumber = r.allocNumber;
	}

	void Dump(FILE *f, std::map<std::string, int> &addr2line) const {
		fprintf(f, "thread=0x%ld\n", this->thread_id);
		for (size_t i = 0; i < this->stacktrace->size(); ++i)
			stacktrace->at(i).Dump(f, addr2line);
	}
};

inline static bool operator < (const TraceInfo &a, const TraceInfo &b) {
	size_t sza = a.stacktrace->size();
	size_t szb = b.stacktrace->size();

	if (sza < szb) return true;
	if (sza > szb) return false;

	for (size_t i = 0; i < sza; ++i) {
		if (a.stacktrace->at(i) < b.stacktrace->at(i)) return true;
		if (a.stacktrace->at(i) > b.stacktrace->at(i)) return false;
	}
	return false;
}

static void init_trace(void) __attribute__((constructor));
static void dump_trace(void) __attribute__((destructor));

static pthread_mutex_t S_mutex;

static const size_t    S_max_frame_depth = 128;
static volatile bool   S_traceOn = true;
static volatile int    S_allocation_count = 0;
static std::map<void *, std::shared_ptr<TraceInfo>> *S_allocations = nullptr;
static char S_fileDump[512] = {0, };
static int  S_break = 0;

static void (*S_cbFreeFailed)() = nullptr;
static void (*S_cbAlloc)(int allocNumber) = nullptr;

static void* (*sys_malloc)(size_t) = nullptr;
static void  (*sys_free)(void*) = nullptr;
static void* (*sys_realloc)(void *ptr, size_t size) = nullptr;
static void* (*sys_calloc)(size_t nitems, size_t size) = nullptr;


static inline std::map<void *, std::shared_ptr<TraceInfo>> * T() {
	if (S_allocations == nullptr)
		S_allocations = new std::map<void *, std::shared_ptr<TraceInfo>>();
	return S_allocations;
}

//############################################################
//############################################################

static void init_trace(void)
{
	fprintf(stderr, "Starting leakfinder \n");

	pthread_mutexattr_t a;
	pthread_mutexattr_init(&a);
	pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK);
	pthread_mutex_init(&S_mutex, &a);
	pthread_mutexattr_destroy(&a);

	int rc = pthread_mutex_lock(&S_mutex);
	if (rc != 0) {
		fprintf(stderr, "Error %d on pthread_mutex_lock\n", rc);
		abort();
	}
	if (true) {

		S_allocations = NULL;
		S_allocation_count = 0;
		S_traceOn = false;
		S_break = 0;
		S_fileDump[0] = 0;

		if (true) {
			void *r = malloc(1);
			r = realloc(r, 2);
			free(r);
			r = calloc(1,1);
			free(r);
		}

		const char *p = getenv("LEAKFINDER");
		if (p != nullptr) {
			bool append_flag = false;
			std::string logFile;
			int cnt = 0;

			ParseArgs(p, append_flag, logFile, cnt);

			strcpy(S_fileDump, logFile.c_str());
			S_break = cnt;

			if (append_flag == false && S_fileDump[0] != 0) {
				FILE *f = fopen(S_fileDump, "w");
				if (f != nullptr)
					fclose(f);
			}
		}

		p = getenv("LD_PRELOAD");
		if (p != nullptr && strstr(p, "leakfinder.so") != nullptr)
			S_traceOn = true;
		else
			S_traceOn = false;
	}
	pthread_mutex_unlock(&S_mutex);
}

static FILE *openFile() {
	FILE *f = stderr;
	if (S_fileDump[0]) {
		f = fopen(S_fileDump, "a");
		if (f == nullptr)
			f = stderr;
	}
	return f;
}
static void closeFile(FILE *f) {
	if (f != stderr)
		fclose(f);
}
static void Break(std::shared_ptr<TraceInfo> ti) {
	std::map<std::string, int> addr2line;
	ti->Dump(stderr, addr2line);
	printf("\nProcess %d\n", getpid());
	char bbb[20];
	fgets(bbb,20,stdin);
}

static std::shared_ptr<std::vector<CallInfo>> DumpStack() {
	void * frames[S_max_frame_depth];
	size_t stack_size = backtrace(frames, S_max_frame_depth);
	char **stacktrace = backtrace_symbols(frames, stack_size);

	std::shared_ptr<std::vector<CallInfo>> r(new std::vector<CallInfo>);
	for (size_t i = 0; i < stack_size; ++i) {
		if (strstr(stacktrace[i], "leakfinder.so") != nullptr) continue;
		CallInfo si;
		si.trace = stacktrace[i];
		r->push_back(si);
	}

	sys_free(stacktrace);

	return r;
}

static void DumpStack(FILE *f) {
	auto r = DumpStack();
	std::map<std::string, int> addr2line;
	for (size_t i = 0; i < r->size(); ++i)
		r->at(i).Dump(f, addr2line);
}

extern "C" {

	LEAKFINDER_API void* malloc(size_t size)
	{
		if (sys_malloc==0)
			sys_malloc = reinterpret_cast<void*(*)(size_t)>(dlsym(RTLD_NEXT, "malloc"));
		void *ptr = sys_malloc(size);

		if (S_traceOn && pthread_mutex_lock(&S_mutex) != EDEADLK) {

			if (ptr != nullptr) {
				auto ai = T();
				auto rr = DumpStack();

				std::shared_ptr<TraceInfo> allocation(new TraceInfo(ptr, size, rr, pthread_self(), ++S_allocation_count));
				ai->insert(std::pair<void *, std::shared_ptr<TraceInfo>>(ptr, allocation));
				if (S_cbAlloc) S_cbAlloc(S_allocation_count);
				if (S_allocation_count == S_break) Break(allocation);
			}

			pthread_mutex_unlock(&S_mutex);
		}
		return ptr;
	}

	LEAKFINDER_API void free(void* ptr) 
	{
		if (sys_free == nullptr)
			sys_free = reinterpret_cast<void(*)(void *)>(dlsym(RTLD_NEXT, "free"));

		if (S_traceOn && pthread_mutex_lock(&S_mutex) != EDEADLK) {

			if (ptr) 
			{
				auto ai = T();
				auto it = ai->find(ptr);
				if (it != ai->end())
					ai->erase(it);
				else {
					FILE *f = openFile();
					fprintf(f, "leakfinder - free(%p) called with an invalid pointer\n", ptr);
					DumpStack(f);
					closeFile(f);

					if (S_cbFreeFailed)
						S_cbFreeFailed();
				}
			}

			pthread_mutex_unlock(&S_mutex);
		}

		sys_free(ptr);
	}

	LEAKFINDER_API void *realloc(void *p, size_t sz) {

		if (sys_realloc == nullptr)
			sys_realloc = reinterpret_cast<void*(*)(void *, size_t)>(dlsym(RTLD_NEXT, "realloc"));
		void *r = sys_realloc(p, sz);

		if (S_traceOn && pthread_mutex_lock(&S_mutex) != EDEADLK) {
			auto ai = T();

			if (p != nullptr) {
				auto it = ai->find(p);
				if (it != ai->end())
					ai->erase(it);
				else {
					FILE *f = openFile();
					fprintf(f, "leakfinder - realloc(%p, %ld) called with an invalid pointer\n", p, sz);
					DumpStack(f);
					closeFile(f);

					if (S_cbFreeFailed)
						S_cbFreeFailed();
				}
			}

			if (r != nullptr) {
				auto rr = DumpStack();

				std::shared_ptr<TraceInfo> allocation(new TraceInfo(r, sz, rr, pthread_self(), ++S_allocation_count));
				ai->insert(std::pair<void *, std::shared_ptr<TraceInfo>>(r, allocation));
				if (S_cbAlloc) S_cbAlloc(S_allocation_count);
				if (S_allocation_count == S_break) Break(allocation);
			}

			pthread_mutex_unlock(&S_mutex);
		}
		return r;
	}

	static void * tmp_calloc(size_t, size_t) {
		return nullptr;
	}

	LEAKFINDER_API void* calloc(size_t nitems, size_t size) {
		if (sys_calloc == nullptr) {
			sys_calloc = tmp_calloc;
			sys_calloc = reinterpret_cast<void*(*)(size_t, size_t)>(dlsym(RTLD_NEXT, "calloc"));
		}
		void *r =  sys_calloc(nitems, size);

		if (S_traceOn && pthread_mutex_lock(&S_mutex) != EDEADLK) {

			if (r != nullptr) {
				auto ai = T();
				auto rr = DumpStack();

				std::shared_ptr<TraceInfo> allocation(new TraceInfo(r, nitems * size, rr, pthread_self(), ++S_allocation_count));
				ai->insert(std::pair<void *, std::shared_ptr<TraceInfo>>(r, allocation));
				if (S_cbAlloc) S_cbAlloc(S_allocation_count);
				if (S_allocation_count == S_break) Break(allocation);
			}

			pthread_mutex_unlock(&S_mutex);
		}
		return r;
	}
}


static void dump_trace()
{
	if (S_traceOn == false)
		return;

	pthread_mutex_lock(&S_mutex);

	FILE *f = openFile();
	auto ai = T();

	if (ai->empty())
	{
		fprintf(f,  "leakfinder found no leaks, not one of the %d allocations was not released\n", S_allocation_count);
	}
	else
	{
		fprintf(f, "leakfinder detected that %d out of %d allocations was not released\n\n", (int)S_allocations->size(), S_allocation_count);

		struct LeakData {
			int cnt;         // numero di tutti i leak in quel TraceInfo
			size_t size;     // dimensione totale di tutti i leak in quel TraceInfo
			int allocNumber; // il piu' piccolo --> il piu' vecchio, ossia il primo leak fatto in quel TraceInfo

			LeakData() { cnt = 0; size = 0; allocNumber = INT_MAX; }
		};

		// organizzo i dati per TraceInfo
		auto cmp = [](std::shared_ptr<TraceInfo> a, std::shared_ptr<TraceInfo> b) {
			return *a < *b;
		};
		std::map<std::shared_ptr<TraceInfo>, LeakData, std::function<bool(std::shared_ptr<TraceInfo>,std::shared_ptr<TraceInfo>)>> dd(cmp);

		for (auto it : *ai) {
			LeakData r;
			if (dd.find(it.second) != dd.end())
				r = dd[it.second];
			r.cnt += 1;
			r.size += it.second->size;
			r.allocNumber = std::min(r.allocNumber, it.second->allocNumber);
			dd[it.second] = r;
		}

		// ora ordino i TraceInfo per size totale - prima i piu' grossi... fino ai piu' piccoli
		std::vector<std::tuple<std::shared_ptr<TraceInfo>, LeakData>> rr;
		for (auto it : dd)
			rr.push_back(std::make_tuple(it.first, it.second));
		auto cmp2 = [] (const std::tuple<std::shared_ptr<TraceInfo>, LeakData> &a, const std::tuple<std::shared_ptr<TraceInfo>, LeakData> &b) { return std::get<1>(a).size > std::get<1>(b).size; };
		std::sort(rr.begin(), rr.end(), cmp2);

		// cache per le chiamate addr2line -e <file> <addr>
		std::map<std::string, int> addr2line;
		for (auto it : rr) {
			if (std::get<1>(it).cnt == 1)
				fprintf(f, "%d leak found - size %ld - alloc number %d - ", std::get<1>(it).cnt, std::get<1>(it).size, std::get<1>(it).allocNumber);
			else
				fprintf(f, "%d leaks found - total size %ld - first alloc number %d - ", std::get<1>(it).cnt, std::get<1>(it).size, std::get<1>(it).allocNumber);

			std::get<0>(it)->Dump(f, addr2line);
		}
	}

	closeFile(f);

	delete S_allocations;
	S_allocations = nullptr;
	S_allocation_count = 0;
	S_traceOn = false;

	pthread_mutex_unlock(&S_mutex);
}


extern "C" {

	LEAKFINDER_API void LeakFinder_StartTrace() {
		pthread_mutex_lock(&S_mutex);

		delete S_allocations;
		S_allocations = nullptr;
		S_allocation_count = 0;
		S_traceOn = true;

		pthread_mutex_unlock(&S_mutex);
	}

	LEAKFINDER_API void LeakFinder_StopTrace() {
		dump_trace();
		S_traceOn = false;
	}

	LEAKFINDER_API void LeakFinder_SetCallbacks(void (*cbFreeFailed)(), void (*cbAlloc)(int)) {
		S_cbFreeFailed = cbFreeFailed;
		S_cbAlloc = cbAlloc;
	}
}
