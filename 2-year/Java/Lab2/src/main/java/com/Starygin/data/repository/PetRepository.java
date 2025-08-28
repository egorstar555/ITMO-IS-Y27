package com.Starygin.data.repository;

import com.Starygin.data.entity.Pet;
import jakarta.persistence.EntityManager;

public class PetRepository extends BaseRepository<Pet> {
    public PetRepository(EntityManager entityManager) {
        super(entityManager, Pet.class);
    }
}
