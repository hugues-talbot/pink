# -*- rpm-spec -*-
Buildroot:      /home/ujoimro/tmp/build/_CPack_Packages/Linux-Source/RPM/BUILD
Summary:        Pink image
Name:           pink
Version:        0.9.r417
Release:        1
License:        CeCILL
Source:         pink-0.9.r417-Source.tar.bz2
Group:          unknown
Vendor:         Esiee Engineering
Packager:       Laszlo Marak <ujoimro@gmail.com>



%define prefix /usr
%define _rpmdir /home/ujoimro/tmp/build/_CPack_Packages/Linux-Source/RPM
%define _rpmfilename pink-0.9.r417-Source.rpm
%define _unpackaged_files_terminate_build 0
%define _topdir /home/ujoimro/tmp/build/_CPack_Packages/Linux-Source/RPM
#%define RPM_BUILD_ROOT %{_topdir}/%{name}-%{version}-root


%description
DESCRIPTION
===========

UjoImro, 2011 
WRITE ME!!!


%prep
echo "RPM BUILDROOT IS " ${RPM_BUILD_ROOT}
#exit(1)
rm -rf $RPM_BUILD_DIR/pink-0.9.r417
rm -rf $RPM_BUILD_DIR/pink-0.9.r417-Build

tar xvf $RPM_SOURCE_DIR/pink-0.9.r417-Source.tar.bz2
mkdir $RPM_BUILD_DIR/pink-0.9.r417-Build
cd $RPM_BUILD_DIR/pink-0.9.r417-Build
cmake -DPYTHON_FRONT_END=ON -DCMAKE_INSTALL_PREFIX=${RPM_BUILD_ROOT}/usr -DPYTHON_MODULE_INSTALL_PATH=${RPM_BUILD_ROOT}/usr/lib/python2.6/site-packages  $RPM_BUILD_DIR/pink-0.9.r417-Source
### We need a second configuration for the python include files
#cmake -DPYTHON_FRONT_END=ON -DCMAKE_INSTALL_PREFIX=$RPM_BUILD_ROOT -DPYTHON_MODULE_INSTALL_PATH=${RPM_BUILD_ROOT}/usr/lib/python2.6/site-packages  $RPM_BUILD_DIR/pink-0.9.r417-Source
cmake .

%build
make -j8 -C $RPM_BUILD_DIR/pink-0.9.r417-Build
  
%install
make -j8 -C $RPM_BUILD_DIR/pink-0.9.r417-Build install

%clean


%files
/usr/lib64/libpink.so
/usr/lib/python2.6/site-packages/libcpp_pink.so
/usr/lib/python2.6/site-packages/pink
/usr/lib/python2.6/site-packages/pink/python_pink.py
/usr/lib/python2.6/site-packages/pink/isize.py
/usr/lib/python2.6/site-packages/pink/render.py
/usr/lib/python2.6/site-packages/pink/long2byte.py
/usr/lib/python2.6/site-packages/pink/cpp
/usr/lib/python2.6/site-packages/pink/cpp/__init__.py
/usr/lib/python2.6/site-packages/pink/pil_photoimage.py
/usr/lib/python2.6/site-packages/pink/MICCAI
/usr/lib/python2.6/site-packages/pink/MICCAI/back_to_dicom.py
/usr/lib/python2.6/site-packages/pink/MICCAI/convert_dicom.py
/usr/lib/python2.6/site-packages/pink/MICCAI/generate_moments.py
/usr/lib/python2.6/site-packages/pink/MICCAI/update.py
/usr/lib/python2.6/site-packages/pink/MICCAI/create_non_isotropized.py
/usr/lib/python2.6/site-packages/pink/MICCAI/get_a_point.py
/usr/lib/python2.6/site-packages/pink/MICCAI/isotropization.py
/usr/lib/python2.6/site-packages/pink/MICCAI/move_the_rest.py
/usr/lib/python2.6/site-packages/pink/MICCAI/registration.py
/usr/lib/python2.6/site-packages/pink/MICCAI/sage2python.py
/usr/lib/python2.6/site-packages/pink/MICCAI/__init__.py
/usr/lib/python2.6/site-packages/pink/MICCAI/headers_to_4D.py
/usr/lib/python2.6/site-packages/pink/MICCAI/itk2pink.py
/usr/lib/python2.6/site-packages/pink/manipulate.py
/usr/lib/python2.6/site-packages/pink/maxflow.py
/usr/lib/python2.6/site-packages/pink/seuilmesh.py
/usr/lib/python2.6/site-packages/pink/zoom.py
/usr/lib/python2.6/site-packages/pink/graph_plot.py
/usr/lib/python2.6/site-packages/pink/drawcurve.py
/usr/lib/python2.6/site-packages/pink/imview.py
/usr/lib/python2.6/site-packages/pink/parallel_map.py
/usr/lib/python2.6/site-packages/pink/__init__.py
/usr/lib/python2.6/site-packages/pink/show.py
/usr/lib/python2.6/site-packages/pink/view3d.py


#%defattr(-,root,root,-)
#%dir %{prefix}
#%{prefix}/*


%changelog
