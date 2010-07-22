/*****************************************************************************\
 *  src/common/unsetenv.c - Kludge for unsetenv on AIX
 *****************************************************************************
 *  Copyright (C) 2004 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov>.
 *  UCRL-CODE-217948.
 *  
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://www.llnl.gov/linux/slurm/>.
 *  
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *  
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
\*****************************************************************************/

#include <stdlib.h>
#include <string.h>

extern int unsetenv (const char *name)
{
	int len, rc;
	char *tmp;

	if (!getenv(name))	/* Nothing to clear */
		return 0;

	len = strlen(name);
	tmp = malloc(len + 3);
	strcpy(tmp, name);
	strcat(tmp, "=x");
	if ((rc = putenv(tmp)) != 0)
		return rc;

	/* Here's the real kludge, just clear the variable out.
	 * This does result in a memory leak. */
	tmp[0] = '\0';
	return 0;
}