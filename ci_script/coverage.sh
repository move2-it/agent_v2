if [ $1 ]; then
    echo "build end run arg"
    qmake Agent2It.pro "GCOVERAGE=true"
    make -j
    ./build/out/AgentTest/AgentTest
fi

lcov --rc lcov_branch_coverage=1 --directory . --capture --output-file cov_full.info
lcov --rc lcov_branch_coverage=1 --extract cov_full.info '*AgentApp*' -o cov.info
lcov --rc lcov_branch_coverage=1 --remove cov.info '*/AgentApp/QtWrappers*' -o cov.info

genhtml --rc lcov_branch_coverage=1 -o results/ cov.info
