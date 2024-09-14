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
#include "../io/log.hpp"
#include "../crypto/random.hpp"
#include "../crypto/sha1.hpp"
#include <map>
#include <stdexcept>

namespace OpenWars {
	struct i_audit_t {
		u32			res = AUDITOR_RESOURCES::MISC;
		void		*ptr = nullptr;
		const char	*what = nullptr;
	};

	typedef std::map<u64, i_audit_t> i_audits_t;

	uintptr_t i_audits = (uintptr_t)nullptr;

	i8 init_auditor(const char *err) {
		if(err != nullptr)
			return -1;

		if(i_audits != (uintptr_t)nullptr) {
			err = "Auditor is already loaded";
			return -1;
		}

		i_audits_t *p = valloc<i_audits_t>(err);

		if(p == nullptr) {
			err = "Couldn't allocate the auditor";
			return -1;
		}

		p->clear();
		i_audits = (uintptr_t)p;

		return 0;
	};

	void deinit_auditor(void) {
		if(i_audits == (uintptr_t)nullptr)
			return;

		i_audits_t *p = (i_audits_t *)i_audits;

		if(p->empty()) {
			log_debug("Auditor is happy :D\n");
		} else {
			log_debug("Auditor is not happy >:( because there are still %u elements that were not de-audited:\n", p->size());

			u64 i = 0;
			for(auto it = p->begin(); it != p->end(); it++, i++) {
				// [TODO] Free?

				log_debug("* Element %lu:\n", i + 1);
				log_debug("  ID: %016lx\n", it->first);
				log_debug("  Resource: %08x\n", it->second.res);
				log_debug("  What: %s\n", it->second.what);
			};
		}

		free(p);
		i_audits = (uintptr_t)nullptr;
	};

	u64 audit(const char *err) {
		if(err != nullptr)
			return -1;

		if(i_audits == (uintptr_t)nullptr) {
			err = "Auditor is not loaded";
			return -1;
		}

		i_audits_t *p = (i_audits_t *)i_audits;

		PRNG prng;
		prng.seed_from_date();

		u64 id = prng.random_u64();

		i_audit_t t;
		p->insert_or_assign(id, t);

		return id;
	};

	u64 audit(u32 res, const char *add, void *addr, const char *err) {
		if(err != nullptr)
			return -1;

		if(i_audits == (uintptr_t)nullptr) {
			err = "Auditor is not loaded";
			return -1;
		}

		i_audits_t *p = (i_audits_t *)i_audits;

		u64 len;
		for(len = 0; (add[len] != '\0'); len++);

		u8 data[] = {
			(u8)(res >> 24),
			(u8)(res >> 16),
			(u8)(res >> 8),
			(u8)res,
		};

		Crypto::SHA1 sha1;
		sha1.init();
		sha1.update(data, 4);
		sha1.update((u8 *)add, len);

		u8 *h = sha1.digest();

		u64 id =	((((u64)(h[0])) << 56) |
					(((u64)(h[1])) << 48) |
					(((u64)(h[2])) << 40) |
					(((u64)(h[3])) << 32) |
					(((u64)(h[4])) << 24) |
					(((u64)(h[5])) << 16) |
					(((u64)(h[6])) << 8) |
					((u64)(h[7])));

		i_audit_t t;
		t.res = res;
		t.ptr = addr;
		t.what = add;

		p->insert_or_assign(id, t);

		return id;
	};

	u64 audit(u32 res, const char *add, const char *err) {
		return audit(res, add, nullptr, err);
	};

	i8 deaudit(u64 id, const char *err) {
		if(err != nullptr)
			return -1;

		if(i_audits == (uintptr_t)nullptr) {
			err = "Auditor is not loaded";
			return -1;
		}

		i_audits_t *p = (i_audits_t *)i_audits;

		try {
			(void)(p->at(id));
		} catch(std::out_of_range &e) {
			err = "Auditor couldn't find that resource";
			return -1;
		};

		p->erase(id);

		return 0;
	};

	bool _is_auditor_online(void) {
		return (i_audits != (uintptr_t)nullptr);
	};
};

#endif
