/*
*         OpenPBS (Portable Batch System) v2.3 Software License
* 
* Copyright (c) 1999-2000 Veridian Information Solutions, Inc.
* All rights reserved.
* 
* ---------------------------------------------------------------------------
* For a license to use or redistribute the OpenPBS software under conditions
* other than those described below, or to purchase support for this software,
* please contact Veridian Systems, PBS Products Department ("Licensor") at:
* 
*    www.OpenPBS.org  +1 650 967-4675                  sales@OpenPBS.org
*                        877 902-4PBS (US toll-free)
* ---------------------------------------------------------------------------
* 
* This license covers use of the OpenPBS v2.3 software (the "Software") at
* your site or location, and, for certain users, redistribution of the
* Software to other sites and locations.  Use and redistribution of
* OpenPBS v2.3 in source and binary forms, with or without modification,
* are permitted provided that all of the following conditions are met.
* After December 31, 2001, only conditions 3-6 must be met:
* 
* 1. Commercial and/or non-commercial use of the Software is permitted
*    provided a current software registration is on file at www.OpenPBS.org.
*    If use of this software contributes to a publication, product, or
*    service, proper attribution must be given; see www.OpenPBS.org/credit.html
* 
* 2. Redistribution in any form is only permitted for non-commercial,
*    non-profit purposes.  There can be no charge for the Software or any
*    software incorporating the Software.  Further, there can be no
*    expectation of revenue generated as a consequence of redistributing
*    the Software.
* 
* 3. Any Redistribution of source code must retain the above copyright notice
*    and the acknowledgment contained in paragraph 6, this list of conditions
*    and the disclaimer contained in paragraph 7.
* 
* 4. Any Redistribution in binary form must reproduce the above copyright
*    notice and the acknowledgment contained in paragraph 6, this list of
*    conditions and the disclaimer contained in paragraph 7 in the
*    documentation and/or other materials provided with the distribution.
* 
* 5. Redistributions in any form must be accompanied by information on how to
*    obtain complete source code for the OpenPBS software and any
*    modifications and/or additions to the OpenPBS software.  The source code
*    must either be included in the distribution or be available for no more
*    than the cost of distribution plus a nominal fee, and all modifications
*    and additions to the Software must be freely redistributable by any party
*    (including Licensor) without restriction.
* 
* 6. All advertising materials mentioning features or use of the Software must
*    display the following acknowledgment:
* 
*     "This product includes software developed by NASA Ames Research Center,
*     Lawrence Livermore National Laboratory, and Veridian Information 
*     Solutions, Inc.
*     Visit www.OpenPBS.org for OpenPBS software support,
*     products, and information."
* 
* 7. DISCLAIMER OF WARRANTY
* 
* THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT
* ARE EXPRESSLY DISCLAIMED.
* 
* IN NO EVENT SHALL VERIDIAN CORPORATION, ITS AFFILIATED COMPANIES, OR THE
* U.S. GOVERNMENT OR ANY OF ITS AGENCIES BE LIABLE FOR ANY DIRECT OR INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* This license will be governed by the laws of the Commonwealth of Virginia,
* without reference to its choice of law rules.
*/
/* 
 *
 * qalter - (PBS) alter batch job
 *
 * Authors:
 *      Terry Heidelberg
 *      Livermore Computing
 *
 *      Bruce Kelly
 *      National Energy Research Supercomputer Center
 *
 *      Lawrence Livermore National Laboratory
 *      University of California
 */

#include "cmds.h"
#include <pbs_config.h>   /* the master config generated by configure */

int main(

  int    argc,
  char **argv)

  {
  int c;
  int errflg=0;
  int any_failed=0;
  char *pc;
  int i;
  int u_cnt, o_cnt, s_cnt, n_cnt;
  struct attrl *attrib = NULL;
  char *keyword;
  char *pdepend;
  char *valuewd;
  time_t after;
  char a_value[80];

  char job_id[PBS_MAXCLTJOBID];

  char job_id_out[PBS_MAXCLTJOBID];
  char server_out[MAXSERVERNAME];
  char rmt_server[MAXSERVERNAME];
  char path_out[MAXPATHLEN + 1];

#define GETOPT_ARGS "a:A:c:e:h:j:k:l:m:M:N:o:p:r:S:u:W:x:"
                 
  while ((c = getopt(argc,argv,GETOPT_ARGS)) != EOF)
    {
    switch (c) 
      {
      case 'a':

        if ((after = cvtdate(optarg)) < 0 ) 
          {
          fprintf(stderr, "qalter: illegal -a value\n");
          errflg++;

          break;
          }

        sprintf(a_value,"%ld", 
          (long)after);

        set_attr(&attrib,ATTR_a,a_value);

        break;

      case 'A':

        set_attr(&attrib, ATTR_A, optarg);

        break;

      case 'c':

        while (isspace((int)*optarg)) 
          optarg++;

        pc = optarg;

        if (strlen(pc) == 1) 
          {
          if ((*pc != 'n') && (*pc != 's') && (*pc != 'c')) 
            {
            fprintf(stderr,"qalter: illegal -c value\n");

            errflg++;

            break;
            }
          } 
        else 
          {
          if (strncmp(pc,"c=",2) != 0) 
            {
            fprintf(stderr,"qalter: illegal -c value\n");

            errflg++;

            break;
            }

          pc += 2;

          if (*pc == '\0') 
            {
            fprintf(stderr,"qalter: illegal -c value\n");

            errflg++;

            break;
            }

          while (isdigit(*pc)) 
            pc++;

          if (*pc != '\0') 
            {
            fprintf(stderr,"qalter: illegal -c value\n");

            errflg++;

            break;
            }
          }

        set_attr(&attrib,ATTR_c,optarg);

        break;

      case 'e':

            if ( prepare_path(optarg, path_out) == 0 ) {
                set_attr(&attrib, ATTR_e, path_out);
            } else {
                fprintf(stderr, "qalter: illegal -e value\n");
                errflg++;
            }
	    break;
        case 'h':
	    while ( isspace((int)*optarg) ) optarg++;
            if ( strlen(optarg) == 0 ) {
                fprintf(stderr, "qalter: illegal -h value\n");
                errflg++;
		break;
            }
	    pc = optarg;
	    u_cnt = o_cnt = s_cnt = n_cnt = 0;
            while ( *pc) {
	        if ( *pc == 'u' )
		    u_cnt++;
                else if ( *pc == 'o' )
		    o_cnt++;
                else if ( *pc == 's' )
		    s_cnt++;
                else if ( *pc == 'n' )
		    n_cnt++;
		else {
		    fprintf(stderr, "qalter: illegal -h value\n");
		    errflg++;
		    break;
		}
                pc++;
            }
            if ( n_cnt && (u_cnt + o_cnt + s_cnt) ) {
                fprintf(stderr, "qalter: illegal -h value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_h, optarg);
            break;
        case 'j':
	    if ( strcmp(optarg, "oe") != 0 &&
		 strcmp(optarg, "eo") != 0 &&
		 strcmp(optarg,  "n") != 0 ) {
		fprintf(stderr, "qalter: illegal -j value\n");
		errflg++;
		break;
	    }
            set_attr(&attrib, ATTR_j, optarg);
            break;
        case 'k':
            if ( strcmp(optarg,  "o") != 0 &&
                 strcmp(optarg,  "e") != 0 &&
                 strcmp(optarg, "oe") != 0 &&
                 strcmp(optarg, "eo") != 0 &&
                 strcmp(optarg,  "n") != 0 ) {
                fprintf(stderr, "qalter: illegal -k value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_k, optarg);
            break;

      case 'l':

        if (set_resources(&attrib,optarg,TRUE)) 
          {
          fprintf(stderr,"qalter: illegal -l value\n");

          errflg++;
	  }

        break;

      case 'm':

        while (isspace((int)*optarg)) 
          optarg++;

            if ( strlen(optarg) == 0 ) {
                fprintf(stderr, "qalter: illegal -m value\n");
                errflg++;
		break;
            }
            if ( strcmp(optarg, "n") != 0 ) {
                pc = optarg;
                while (*pc) {
                    if ( *pc != 'a' && *pc != 'b' && *pc != 'e' ) {
                        fprintf(stderr, "qalter: illegal -m value\n");
                        errflg++;
			break;
                    }
                    pc++;
                }
            }
            set_attr(&attrib, ATTR_m, optarg);
            break;
        case 'M':
            if ( parse_at_list(optarg, FALSE, FALSE) ) {
                fprintf(stderr, "qalter: illegal -M value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_M, optarg);
            break;
        case 'N':
	    if (check_job_name(optarg, 1) == 0) {
		set_attr(&attrib, ATTR_N, optarg);
	    } else {
		fprintf(stderr, "qalter: illegal -N value\n");
		errflg++;
	    }
            break;
        case 'o':
            if ( prepare_path(optarg, path_out) == 0 ) {
                set_attr(&attrib, ATTR_o, path_out);
            } else {
                fprintf(stderr, "qalter: illegal -o value\n");
                errflg++;
            }
	    break;
        case 'p':
	    while ( isspace((int)*optarg) ) optarg++;
            pc = optarg;
            if ( *pc == '-' || *pc == '+' ) pc++;
            if ( strlen(pc) == 0 ) {
                fprintf(stderr, "qalter: illegal -p value\n");
                errflg++;
		break;
            }
            while ( *pc != '\0' ) {
                if ( ! isdigit(*pc) ) {
                    fprintf(stderr, "qalter: illegal -p value\n");
                    errflg++;
		    break;
                }
                pc++;
            }
            i = atoi(optarg);
            if ( i < -1024 || i > 1023 ) {
                fprintf(stderr, "qalter: illegal -p value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_p, optarg);
            break;
        case 'r':
            if ( strlen(optarg) != 1 ) {
                fprintf(stderr, "qalter: illegal -r value\n");
                errflg++;
		break;
            }
            if ( *optarg != 'y' && *optarg != 'n' ) {
                fprintf(stderr, "qalter: illegal -r value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_r, optarg);
            break;
        case 'S':
            if ( parse_at_list(optarg, TRUE, TRUE) ) {
                fprintf(stderr, "qalter: illegal -S value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_S, optarg);
            break;
        case 'u':
            if ( parse_at_list(optarg, TRUE, FALSE) ) {
                fprintf(stderr, "qalter: illegal -u value\n");
                errflg++;
		break;
            }
            set_attr(&attrib, ATTR_u, optarg);
            break;
	case 'W':
            while ( isspace((int)*optarg) ) optarg++;
            if ( strlen(optarg) == 0 ) {
                fprintf(stderr, "qalter: illegal -W value\n");
                errflg++;
		break;
            }
	    i = parse_equal_string(optarg, &keyword, &valuewd);
	    while (i == 1) {
		if (strcmp(keyword, ATTR_depend) == 0) {
		    pdepend = malloc(PBS_DEPEND_LEN);
                    if ( parse_depend_list(valuewd, pdepend, PBS_DEPEND_LEN) ) {
                        fprintf(stderr, "qalter: illegal -W value\n");
                        errflg++;
			break;
                    }
		    valuewd = pdepend;
		} else if (strcmp(keyword, ATTR_stagein) == 0) {
                    if ( parse_stage_list(valuewd) ) {
                        fprintf(stderr, "qalter: illegal -W value\n");
                        errflg++;
			break;
                    }
		} else if (strcmp(keyword, ATTR_stageout) == 0) {
                    if ( parse_stage_list(valuewd) ) {
                        fprintf(stderr, "qalter: illegal -W value\n");
                        errflg++;
			break;
                    }
		} else if (strcmp(keyword, ATTR_g) == 0) {
                    if ( parse_at_list(valuewd, TRUE, FALSE) ) {
                        fprintf(stderr, "qalter: illegal -W value\n");
                        errflg++;
			break;
                    }
		}
		set_attr(&attrib, keyword, valuewd);
		i = parse_equal_string((char *)0, &keyword, &valuewd);
	    }
	    if (i == -1) {
                fprintf(stderr, "qalter: illegal -W value\n");
                errflg++;
	    }
        break;

      case 'x':

        /* exec_hosts */

        set_attr(&attrib,ATTR_exechost,optarg);

        break;

      case '?':
      default:

        errflg++;

        break;
      }  /* END switch(c) */
    }    /* END while(c = getopt()) */

  if (errflg || (optind == argc)) 
    {
    static char usage[]="usage: qalter \
[-a date_time] [-A account_string] [-c interval] [-e path] \n\
[-h hold_list] [-j y|n] [-k keep] [-l resource_list] [-m mail_options] \n\
[-M user_list] [-N jobname] [-o path] [-p priority] [-r y|n] [-S path] \n\
[-u user_list] [-W dependency_list] [-x exec_host] job_identifier...\n";

    fprintf(stderr,usage);

    exit(2);
    }

    for ( ; optind < argc; optind++) {
        int connect;
        int stat=0;
	int located = FALSE;

        strcpy(job_id, argv[optind]);
        if ( get_server(job_id, job_id_out, server_out) ) {
            fprintf(stderr, "qalter: illegally formed job identifier: %s\n", job_id);
            any_failed = 1;
            continue;
        }
cnt:
        connect = cnt2server(server_out);
        if ( connect <= 0 ) {
            fprintf(stderr, "qalter: cannot connect to server %s (errno=%d)\n",
                    pbs_server, pbs_errno);
            any_failed = pbs_errno;
            continue;
        }

        stat = pbs_alterjob(connect, job_id_out, attrib, NULL);
        if ( stat && (pbs_errno != PBSE_UNKJOBID) ) {
	    prt_job_err("qalter", connect, job_id_out);
            any_failed = pbs_errno;
        } else if ( stat && (pbs_errno == PBSE_UNKJOBID) && !located ) {
	    located = TRUE;
	    if ( locate_job(job_id_out, server_out, rmt_server) ) {
	        pbs_disconnect(connect);
		strcpy(server_out, rmt_server);
		goto cnt;
	    }
	    prt_job_err("qalter", connect, job_id_out);
	    any_failed = pbs_errno;
	}

        pbs_disconnect(connect);
    }
    exit(any_failed);
}
