/**
 *
   ___                __        __
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef __openwars__auditor__cpp__
#define __openwars__auditor__cpp__

#include "auditor.hpp"
#include "random.hpp"
#include "../io/log.hpp"
#include <new>
#include <map>
#include <stdexcept>

namespace OpenWars {
	u64 p_audits_hash64(const char *s) {
		Random rnd;
		rnd.init();

		u64 h = 3141592653589793238;
		u64 len = 1;
		
		while(*s != '\0') {
			rnd.seed((u32)(*s));

			h <<= 1;
			h ^= (u64)(rnd.random_u32());
			h ^= 32;
			h ^= (u64)(rnd.random_u32());

			s++;
			len++;
		};

		h ^= len;
		h ^= (u64)(-1);

		h ^= (u64)(rnd.random_u32());
		h ^= 32;
		h ^= (u64)(rnd.random_u32());

		return h;
	};

	typedef struct {
		u32	res;
		u32	id;
	} p_audit_t;

	typedef std::map<u64, u8> p_audits_t;

	uintptr_t p_audits = (uintptr_t)nullptr;

	ErrorOr<void> init_auditor(void) {
		if(p_audits != (uintptr_t)nullptr)
			return Error { "Auditor is not loaded" };

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

	ErrorOr<u64> audit(u32 res) {
		if(p_audits == (uintptr_t)nullptr)
			return Error { "Auditor is not loaded" };

		p_audits_t *p = (p_audits_t *)p_audits;

		Random rnd;
		rnd.seed_from_date();

		u64 h = (u64)(rnd.random_u32());
		h <<= 32;
		h |= (u64)(rnd.random_u32());

		p->insert_or_assign(h, 0x5f);

		return h;
	};

	ErrorOr<u64> audit(u32 res, const char *add) {
		if(p_audits == (uintptr_t)nullptr)
			return Error { "Auditor is not loaded" };

		p_audits_t *p = (p_audits_t *)p_audits;

		u64 h = p_audits_hash64(add);
		h ^= (u64)res;

		p->insert_or_assign(h, 0x5f);

		return h;
	};

	ErrorOr<void> deaudit(u64 id) {
		if(p_audits == (uintptr_t)nullptr)
			return Error { "Auditor is not loaded" };

		p_audits_t *p = (p_audits_t *)p_audits;

		u8 r;

		try {
			r = p->at(id);
		} catch(std::out_of_range &e) {
			return Error { "Auditor couldn't find that resource" };
		};

		if(r != 0x5f)
			return Error { "Auditor couldn't check(?) that resource" };

		p->insert_or_assign(id, 0xab);
		p->erase(id);

		return Error { nullptr };
	};

	ErrorOr<void> deaudit(u32 res, const char *add) {
		u64 h = p_audits_hash64(add);
		h ^= (u64)res;

		return deaudit(h);
	};
};

#endif
