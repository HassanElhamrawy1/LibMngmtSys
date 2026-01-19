#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User 
{
	public:
		Admin(const std::string& username, const std::string& password)
			: User(username, password) {}

    // Admin-specific functions can go here
};

#endif
