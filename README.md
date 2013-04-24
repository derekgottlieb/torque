# TORQUE Resource Manager

This is the master branch README. Major changes and updates should go into
this branch.

- [Homepage](http://www.adaptivecomputing.com/products/open-source/torque/)
- [Documentation](http://docs.adaptivecomputing.com/)
- [Mailing list](mailto:torqueusers@supercluster.org)

## Overview

TORQUE (Terascale Open-source Resource and Queue manger) is an open source
project based on the original PBS* resource manager developed by NASA, 
LLNL, and MRJ. It possesses a large number of enhancements contributed by 
organizations such as OSC, NCSA, TeraGrid, the U.S Dept of Energy, USC, and 
many, many others. It continues to incorporate significant advancements in
the areas of scalability, fault-tolerance, usability, functionality, and 
security development from the community and vendor supporters. It may be 
utilized, modified, and distributed subject to the constraints of the license 
located in the PBS_License.txt file.  If you would like to contribute to this 
project or have patches, enhancements, or associated projects you would like 
to have included in this project, please send an email to our [mailing list](mailto:torqueusers@supercluster.org).

## Installation Instructions

The current build status is [![Build Status](https://travis-ci.org/adaptivecomputing/torque.png?branch=master)](https://travis-ci.org/adaptivecomputing/torque).

Install directions are available on [the documentation website](http://docs.adaptivecomputing.com/)
  
Additional information concerning TORQUE is available in the PBS
Administrator's Guide available in the admin_guide.ps found in the 'doc'
subdirectory.

## Additional Notes

- TORQUE is not endorsed by nor affiliated with Altair.


---

# Tracking the main TORQUE development repo

Once you've checked out my repo, we'll need to add a remote branch that points to the original repo we forked from:

`git remote add --track master adaptive https://github.com/adaptivecomputing/torque.git`

We could replace 'master' with another TORQUE branch if we wanted to track a
specific release instead of the master branch (e.g., the 4.2-dev head).  I've
opted to name it 'adaptive' to keep track of it, so you should see that listed
if you type (origin would be my repo):

`git remote`

To fetch all changes from Adaptive's code base:

`git fetch adaptive`

This should create a new remote branch called 'adaptive/master'.  We can then
merge the code from the remote repo:

`git merge adaptive/master`

Alternatively, we could combine the fetch/merge using pull:

`git pull adaptive/master`
