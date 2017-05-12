# Building your own decay

{% objectives "Learning Objectives" %}
* How existing containers of particles can be filtered.
* How new particles are made by combining existing particles.
* How to express particle selections and combinations in options files, the 
  Stripping, and the trigger.
{% endobjectives %}

As you might imagine, combining reconstructed tracks under some physical 
hypothesis is quite a common operation for a particle physicist to perform.

We've already manipulated the result of such an operation, so-called composite 
particles, and in this short series of lessons we'll see how you can create 
such composites yourself.
The knowledge you'll gain will give you the ability to understand the large 
body of existing particle combination and filtering code, as well as the 
ability to use the Stripping and HLT2 selection frameworks to write new 
combinations for your analysis.

{% challenge "Data processing flow" %}
Some charged and neutral particles are created in 'the reconstruction', either 
Brunel or the beginning of HLT1 and HLT2.
These include 'stable' particles like electrons, protons, and charged kaons and 
pions, and neutrals like photons and neutral pions.

Why aren't 'composite' particles, like $$D$$ and $$B$$ mesons, also created in 
Brunel? What are the advantages and disadvantages of creating particle 
combinations in a separate step?
{% endchallenge %}
