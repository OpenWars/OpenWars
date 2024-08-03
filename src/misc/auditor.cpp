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
#include <list>

namespace OpenWars {
	u32 p_audits_hash32(const char *s) {
		Random rnd;
		rnd.init();

		u32 h = 0xffffffff;
		u32 len = 0;
		
		while(*s != '\0') {
			rnd.seed((u32)(*s));
			h <<= 1;
			h ^= rnd.random_u32();
			s++;

			len++;
		};

		h ^= len;
		h ^= rnd.random_u32();

		return h;
	};

	struct p_audit_h128_t {
		u32	b0, b1, b2, b3;

		bool operator==(p_audit_h128_t &other) {
			return (
				(b0 == other.b0) &
				(b1 == other.b1) &
				(b2 == other.b2) &
				(b3 == other.b3)
			);
		};
	};

	p_audit_h128_t p_audits_hash128(const char *s, p_audit_h128_t c) {
		p_audit_h128_t h = {
			3141592653,
			589793238,
			462643383,
			2795028841,
		};

		h.b0 ^= c.b0;
		h.b1 ^= c.b1;
		h.b2 ^= c.b2;
		h.b3 ^= c.b3;

		Random rnd;
		rnd.init();

		u32 len = 0;
		
		while(*s != '\0') {
			rnd.seed((u32)(*s));

			h.b0 <<= 1;
			h.b0 ^= rnd.random_u32();

			h.b1 <<= 3;
			h.b1 ^= rnd.random_u32();

			h.b2 <<= 5;
			h.b2 ^= h.b0;
			h.b2 ^= rnd.random_u32();

			h.b3 <<= 7;
			h.b3 ^= h.b1;
			h.b3 ^= rnd.random_u32();

			s++;
			len++;
		};

		h.b0 ^= len;
		h.b1 += len;
		h.b2 -= len;
		h.b3 ^= len;

		rnd.seed(h.b3);
		h.b0 ^= rnd.random_u32();
		rnd.seed(h.b2);
		h.b1 ^= rnd.random_u32();
		rnd.seed(h.b1);
		h.b2 ^= rnd.random_u32();
		rnd.seed(h.b0);
		h.b3 ^= rnd.random_u32();

		return h;
	};

	p_audit_h128_t p_audits_hash128(const char *s) {
		return p_audits_hash128(s, { 3, 5, 7, 11 });
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
