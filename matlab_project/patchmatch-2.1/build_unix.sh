# Optimized (slow to build)
/usr/local/MATLAB/R2013b/bin/mex  CXXOPTIMFLAGS='-O6 -w -s -ffast-math -fomit-frame-pointer -fstrength-reduce -fopenmp -msse2 -funroll-loops -fPIC' CXXFLAGS='-DNDEBUG -DUNIX_MODE -DMEXMODE -fopenmp' CXXLIBS='${CXXLIBS} -Wl,--export-dynamic -Wl,-e,mexFunction -shared -lgomp'  knn.cpp mexutil.cpp nn.cpp nnmex.cpp patch.cpp vecnn.cpp simnn.cpp allegro_emu.cpp -output nnmex -output nnmex
/usr/local/MATLAB/R2013b/bin/mex CXXOPTIMFLAGS='-O6 -w -s -ffast-math -fomit-frame-pointer -fstrength-reduce -fopenmp -msse2 -funroll-loops -fPIC' CXXFLAGS='-DNDEBUG -DUNIX_MODE -DMEXMODE -fopenmp' CXXLIBS='${CXXLIBS} -Wl,--export-dynamic -Wl,-e,mexFunction -shared -lgomp'  knn.cpp mexutil.cpp nn.cpp votemex.cpp patch.cpp vecnn.cpp simnn.cpp allegro_emu.cpp -output nnmex -output votemex
/usr/local/MATLAB/R2013b/bin/mex CXXOPTIMFLAGS='-O6 -w -s -ffast-math -fomit-frame-pointer -fstrength-reduce -fopenmp -msse2 -funroll-loops -fPIC' CXXFLAGS='-DNDEBUG -DUNIX_MODE -DMEXMODE -fopenmp' CXXLIBS='${CXXLIBS} -Wl,--export-dynamic -Wl,-e,mexFunction -shared -lgomp'  knn.cpp mexutil.cpp nn.cpp patchdist.cpp patch.cpp vecnn.cpp simnn.cpp allegro_emu.cpp -output nnmex -output patchdist
#
# Unoptimized (fast to build)
#mex CXXOPTIMFLAGS='-w -fPIC' CXXFLAGS='-DNDEBUG -DUNIX_MODE -DMEXMODE -fopenmp' CXXLIBS='${CXXLIBS} -Wl,--export-dynamic -Wl,-e,mexFunction -shared -lgomp' knn.cpp mexutil.cpp nn.cpp nnmex.cpp patch.cpp vecnn.cpp simnn.cpp allegro_emu.cpp -output nnmex
#mex CXXOPTIMFLAGS='-w -fPIC' CXXFLAGS='-DNDEBUG -DUNIX_MODE -DMEXMODE -fopenmp' CXXLIBS='${CXXLIBS} -Wl,--export-dynamic -Wl,-e,mexFunction -shared -lgomp' knn.cpp mexutil.cpp nn.cpp votemex.cpp patch.cpp vecnn.cpp simnn.cpp allegro_emu.cpp -output votemex
#mex CXXOPTIMFLAGS='-w -fPIC' CXXFLAGS='-DNDEBUG -DUNIX_MODE -DMEXMODE -fopenmp' CXXLIBS='${CXXLIBS} -Wl,--export-dynamic -Wl,-e,mexFunction -shared -lgomp' knn.cpp mexutil.cpp nn.cpp patchdist.cpp patch.cpp vecnn.cpp simnn.cpp allegro_emu.cpp -output patchdist
