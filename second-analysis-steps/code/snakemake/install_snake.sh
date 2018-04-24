wget https://repo.continuum.io/miniconda/Miniconda-latest-Linux-x86_64.sh
chmod +x Miniconda-latest-Linux-x86_64.sh
export PYTHONPATH=""
./Miniconda-latest-Linux-x86_64.sh -b
export PATH=$HOME/miniconda2/bin:$PATH
conda update -y conda
conda create -n snake -c bioconda python=3.4 snakemake beautiful-soup pyyaml

