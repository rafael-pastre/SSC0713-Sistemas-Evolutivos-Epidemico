# Genetical Algorithm for Epidemic Model parameter ajusting

## The Epidemic Model
When studing an epidemic, many models can be used to describe the disease evolution.
The most common ones are from the SIR family, that divide the total population in groups accordingly to the dynamics of the disease.
These dynamics can be as complex and precise as one wishes, and the parameter fitting of the model can get very complex to do.
So here we propose to use a genetical algorithm to make this parameter ajusting in one of theese models, called the SIRD model.

The SIRD model divides the total population in four groups: The ones who are susceptible to get the disease belong to the "Susceptible" group.
The ones that are curently with the disease and can infect others belongs to the "Infected" or "Infectious" group.
The ones recovered from de disease belongs to the "Recovered" group. And the ones that died from the disease belong to the "Dead" group.
In the SIRD model, we use the letters "S", "I", "R" and "D" to represent the number of people in each group, respectively,
and the dynamics of the model consists of some individuals from "S" going to "I" and then going to "R" or "D".
To represent the rate which theese transitions happen, there are used three parameters β, γ and ε that indicates the rate from "S" to "I", from "I" to "R" and from "I" to "D".
The model and the transitions can de visualized in the equations and the picture bellow:
