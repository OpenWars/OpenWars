#ifndef __openwars__auditor__cpp__
#define __openwars__auditor__cpp__

#include "auditor.hpp"
#include "random.hpp"
#include "../io/log.hpp"
#include <new>
#include <list>

namespace OpenWars {
	u32 p_audits_hash32(const char *s) {
		Random rnd;
		rnd.init();

		u32 h = 0xffffffff;
		
		while(*s != '\0') {
			rnd.seed((u32)(*s));
			h <<= 1;
			h ^= rnd.random_u32();
			s++;
		};

		h ^= rnd.random_u32();

		return h;
	};

	typedef struct {
		u32	res;
		u32	id;
	} p_audit_t;

	typedef std::list<p_audit_t> p_audits_t;

	uintptr_t p_audits = (uintptr_t)nullptr;

	ErrorOr<void> init_auditor(void) {
		if(p_audits != (uintptr_t)nullptr)
			return Error { "Auditor variable is not empty" };

		p_audits_t *p;

		try {
			p = new p_audits_t;
		} catch(std::bad_alloc &e) {
			return Error { "Couldn't allocate the auditor" };
		};

		p->clear();
		p_audits = (uintptr_t)p;

		return Error { nullptr };
	};

	void deinit_auditor(void) {
		if(p_audits == (uintptr_t)nullptr)
			return;

		p_audits_t *p = (p_audits_t *)p_audits;

		if(p->empty()) {
			log_debug("Auditor is happy :D\n");
		} else {
			log_debug("Auditor is not happy, because there are still %u elements that were not de-audited.\n", p->size());
		}

		delete p;
		p_audits = (uintptr_t)nullptr;
	};

	ErrorOr<void> audit(u32 res, const char *add) {
		if(p_audits == (uintptr_t)nullptr)
			return Error { "Auditor is empty" };

		p_audits_t *p = (p_audits_t *)p_audits;

		p->push_back({
			res,
			((p_audits_hash32(add) << 8) | res),
		});

		return Error { nullptr };
	};

	ErrorOr<void> deaudit(u32 res, const char *add) {
		if(p_audits == (uintptr_t)nullptr)
			return Error { "Auditor is empty" };

		p_audits_t *p = (p_audits_t *)p_audits;
		u32 id = ((p_audits_hash32(add) << 8) | res);

		p_audits_t::iterator it = p->begin();
		u32 i = 0;

		while(i < p->size()) {
			if((it->res == res) && (it->id == id)) {
				p->erase(it);
				return Error { nullptr };
			}

			std::advance(it, 1);
		};

		return Error { "Couldn't find that resource" };
	};
};

#endif
