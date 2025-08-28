package com.Starygin.data.repository;

import com.Starygin.data.entity.Owner;
import jakarta.persistence.EntityManager;

public class OwnerRepository extends BaseRepository<Owner> {
    public OwnerRepository(EntityManager entityManager) {
        super(entityManager, Owner.class);
    }
}
